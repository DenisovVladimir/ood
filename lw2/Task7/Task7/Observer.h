#pragma once

#include <set>
#include <functional>
#include <vector>

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
	virtual void Update(T dataType, IObservable<T>* observable) = 0;
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
	virtual void RegisterObserver(T type, IObserver<T>& observer) = 0;
	virtual void NotifyObservers(T type) = 0;
	virtual void RemoveObserver(T type, IObserver<T>& observer) = 0;
};

// Реализация интерфейса IObservable
template <typename T>
class CObservable : public IObservable<T>
{
public:
	using ObserverType = IObserver<T>;

	void RegisterObserver(T type, ObserverType& observer) override
	{
		m_observers[type].insert(&observer);
	}

	void NotifyObservers(T type) override
	{
		std::set<ObserverType*> temp(m_observers[type]);
		for (auto& observer : temp)
		{
			observer->Update(type, this);
		}
	}

	void RemoveObserver(T type, ObserverType& observer) override
	{
		m_observers[type].erase(&observer);
	}
private:
	std::map<T, std::set<ObserverType*>> m_observers;
};