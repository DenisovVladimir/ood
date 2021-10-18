#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <climits>
#include "Observer.h"

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
}; 

struct SWeatherProInfo : public SWeatherInfo
{
	double windSpeed = 0;
	double windDirection = 0;
};

class CWeatherObserverBase : public IObserver<SWeatherInfo>
{
public:
	void SetInStation(IObservable<SWeatherInfo>* observable)
	{
		m_inStation = observable;
	}

	void SetOutStation(IObservable<SWeatherInfo>* observable)
	{
		m_outStation = observable;
	}
protected:
	IObservable<SWeatherInfo>* m_inStation = nullptr;
	IObservable<SWeatherInfo>* m_outStation = nullptr;
};

class CDisplay : public CWeatherObserverBase
{
private:
	void PrintInfo(const std::string& stationType, const SWeatherInfo& data)
	{
		std::cout << "Current Temp " << stationType << ' ' << data.temperature << std::endl;
		std::cout << "Current Hum " << stationType << ' ' << data.humidity << std::endl;
		std::cout << "Current Pressure " << stationType << ' ' << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
	
	
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfo const& data, IObservable<SWeatherInfo>* observable) override
	{
		if (observable == m_inStation)
		{
			PrintInfo("inside", data);
		}
		else if (observable == m_outStation)
		{
			PrintInfo("outside", data);
		}
	}
};

class CStatsDisplay : public CWeatherObserverBase
{
	struct Indication
	{
		Indication(const std::string inputName)
			:name(inputName){}
		std::string name;
		double max = -std::numeric_limits<double>::infinity();
		double min = std::numeric_limits<double>::infinity();
		double acc = 0;
		unsigned countAcc = 0;
	};

	struct AllStationIndications
	{
		Indication temperature = Indication("Temp");
		Indication humidity = Indication("Hum");
		Indication pressure = Indication("Pressure");
	};
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void UpdateIndicationData(double newData, Indication& data)
	{
		if (data.max < newData) { data.max = newData; }
		if (data.min > newData) { data.min = newData; }
		data.acc += newData;
		++data.countAcc;
	}

	void UpdateIndications(const SWeatherInfo& data, AllStationIndications& indications)
	{
		UpdateIndicationData(data.temperature, indications.temperature);
		UpdateIndicationData(data.humidity, indications.humidity);
		UpdateIndicationData(data.pressure, indications.pressure);
	}

	void PrintIndicationData(const std::string& stationType, const Indication& data)
	{
		std::cout << "Max " << data.name << ' ' << stationType << ' ' << data.max << '\n';
		std::cout << "Min " << data.name << ' ' << stationType << ' ' << data.min << '\n';
		std::cout << "Average " << data.name << ' ' << stationType << ' ' << (data.acc / data.countAcc) << '\n';
		std::cout << "----------------\n";
	}

	void PrintIndications(const std::string& stationType, AllStationIndications& indications)
	{
		PrintIndicationData(stationType, indications.temperature);
		PrintIndicationData(stationType, indications.humidity);
		PrintIndicationData(stationType, indications.pressure);
	}

	void Update(SWeatherInfo const& data, IObservable<SWeatherInfo>* observable) override
	{
		if (observable == m_inStation)
		{
			UpdateIndications(data, m_inIndications);
			PrintIndications("inside", m_inIndications);
		}
		else if (observable == m_outStation)
		{
			UpdateIndications(data, m_outIndications);
			PrintIndications("outside", m_outIndications);
		}
	}

	AllStationIndications m_inIndications;
	AllStationIndications m_outIndications;
};

class CWeatherData : public CObservable<SWeatherInfo>
{
public:
	// Температура в градусах Цельсия
	double GetTemperature()const
	{
		return m_temperature;
	}
	// Относительная влажность (0...100)
	double GetHumidity()const
	{
		return m_humidity;
	}
	// Атмосферное давление (в мм.рт.ст)
	double GetPressure()const
	{
		return m_pressure;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;

		MeasurementsChanged();
	}
protected:
	SWeatherInfo GetChangedData()const override
	{
		SWeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
};
