#pragma once

#include <set>
#include <functional>
#include <vector>
#include <map>

template<typename T> class IObservable;

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T>
class IObserver
{
public:
	virtual void Update(T const& data) = 0;
	virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual IObservable<T>* RegisterObserver(IObserver<T>& observer, int priority) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObserver(IObserver<T>& observer) = 0;
};

// Реализация интерфейса IObservable
template <class T>
class CObservable : public IObservable<T>
{
public:
	typedef IObserver<T> ObserverType;

	IObservable<T>* RegisterObserver(ObserverType& observer, int priority) override
	{
		if (m_observers.find(&observer) != m_observers.end())
		{
			return this;
		}
		
		m_observers.insert(&observer);
		try
		{
			m_observersRate.insert({ priority, &observer });
		}
		catch (...)
		{
			m_observers.erase(&observer);
		}
		
		return this;
	}

	void NotifyObservers() override
	{
		std::multimap<int, ObserverType*, std::greater<int>> temp(m_observersRate);
		
		T data = GetChangedData();
		for (auto& observer : temp)
		{
			observer.second->Update(data);
		}
	}

	void RemoveObserver(ObserverType& observer) override
	{
		DeleteFromLists(&observer);
	}

protected:
	// Классы-наследники должны перегрузить данный метод, 
	// в котором возвращать информацию об изменениях в объекте
	virtual T GetChangedData()const = 0;

private:
	void DeleteFromLists(ObserverType* observer)
	{
		if (m_observers.find(observer) == m_observers.end())
		{
			return;
		}

		for (auto i = m_observersRate.begin(); i != m_observersRate.end(); ++i)
		{
			if (i->second == observer)
			{
				m_observersRate.erase(i);
				break;
			}
		}

		m_observers.erase(observer);
	}

	std::set<ObserverType*> m_observers;
	std::multimap<int, ObserverType*, std::greater<int>> m_observersRate;
};
