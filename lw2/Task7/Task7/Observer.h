#pragma once

#include <set>
#include <functional>
#include <vector>

template<typename T> class IObservable;

/*
��������� ��������� IObserver. ��� ������ ������������� �����,
�������� �������� ����������� �� ���������������� IObservable
���������� ������� �������� ��� ���������,
������������� ����������� � ����� Update
*/
template <typename T>
class IObserver
{
public:
	virtual void Update(T dataType, IObservable<T>* observable) = 0;
	virtual ~IObserver() = default;
};

/*
��������� ��������� IObservable. ��������� ����������� � ���������� �� ����������, � �����
������������ �������� ����������� ������������������ ������������.
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

// ���������� ���������� IObservable
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