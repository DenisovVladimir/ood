#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include "Observer.h"

enum class SWeatherInfoType
{
	Temperature,
	Humidity,
	Pressure,
	WindSpeed,
	WindDirection
};

class CWeatherData : public CObservable<SWeatherInfoType>
{
public:
	double GetTemperature() const
	{
		return m_temperature;
	}

	double GetHumidity() const
	{
		return m_humidity;
	}

	double GetPressure() const
	{
		return m_pressure;
	}

	double GetWindSpeed() const
	{
		return m_windSpeed;
	}

	double GetWindDirection() const
	{
		return m_windDirection;
	}
	
	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		SetTemp(temp);
		SetHumidity(humidity);
		SetPressure(pressure);
		SetWindSpeed(windSpeed);
		SetWindDirection(windDirection);
	}
private:
	void SetTemp(double temp)
	{
		if (m_temperature != temp)
		{
			m_temperature = temp;
			NotifyObservers(SWeatherInfoType::Temperature);
		}
	}
	void SetHumidity(double humidity)
	{
		if (m_humidity != humidity)
		{
			m_humidity = humidity;
			NotifyObservers(SWeatherInfoType::Humidity);
		}
	}
	void SetPressure(double pressure)
	{
		if (m_pressure != pressure)
		{
			m_pressure = pressure;
			NotifyObservers(SWeatherInfoType::Pressure);
		}
	}
	void SetWindSpeed(double windSpeed)
	{
		if (m_windSpeed != windSpeed)
		{
			m_windSpeed = windSpeed;
			NotifyObservers(SWeatherInfoType::WindSpeed);
		}
	}
	void SetWindDirection(double windDirection)
	{
		if (m_windDirection != windDirection)
		{
			m_windDirection = windDirection;
			NotifyObservers(SWeatherInfoType::WindDirection);
		}
	}
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 0.0;
	double m_windSpeed = 0.0;
	double m_windDirection = 0.0;
};

class CDisplay : public IObserver<SWeatherInfoType>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	static void PrintInfo(SWeatherInfoType dataType, CWeatherData* weatherData)
	{
		switch (static_cast<int>(dataType))
		{
		case 0:
			std::cout << "Current temp: " << weatherData->GetTemperature() << '\n';
			break;
		case 1:
			std::cout << "Current hum: " << weatherData->GetHumidity() << '\n';
			break;
		case 2:
			std::cout << "Current pressure: " << weatherData->GetPressure() << '\n';
			break;
		case 3:
			std::cout << "Current wind speed: " << weatherData->GetWindSpeed() << '\n';
			break;
		case 4:
			std::cout << "Current wind direction: " << weatherData->GetWindDirection() << '\n';
			break;
		default:
			break;
		}
	}
	void Update(SWeatherInfoType dataType, IObservable<SWeatherInfoType>* observable) override
	{
		auto weatherData = dynamic_cast<CWeatherData*>(observable);
		PrintInfo(dataType, weatherData);
	}
};


class NormalIndication
{
public:
	NormalIndication(const std::string& name)
		:m_name(name) {}

	void Update(double newData)
	{
		m_max = std::max(newData, m_max);
		m_min = std::min(newData, m_min);
		m_acc += newData;
		++m_countAcc;
	}
	void Print() const 
	{
		std::cout << "Max " << m_name << ' ' << m_max << '\n';
		std::cout << "Min " << m_name << ' ' << m_min << '\n';
		std::cout << "Average " << m_name << ' ' << (m_acc / m_countAcc) << '\n';
	}
private:
	std::string m_name;
	double m_max = -std::numeric_limits<double>::infinity();
	double m_min = std::numeric_limits<double>::infinity();
	double m_acc = 0;
	unsigned m_countAcc = 0;
};

class CircIndication
{
public:
	CircIndication(const std::string& name)
		:m_name(name) {}

	const double PI = std::atan(1) * 4;
	void Update(double newData)
	{
		m_max = std::max(newData, m_max);
		m_min = std::min(newData, m_min);
		++m_countAcc;
		m_sinAcc += std::sin(newData * PI / 180);
		m_cosAcc += std::cos(newData * PI / 180);
	}

	void Print() const
	{
		std::cout << "Max " << m_name << ' ' << m_max << '\n';
		std::cout << "Min " << m_name << ' ' << m_min << '\n';
		std::cout << "Average " << m_name << ' '
			<< (std::atan2(m_sinAcc / m_countAcc, m_cosAcc / m_countAcc) * 180 / PI) << '\n';
	}
private:
	std::string m_name;
	double m_max = -std::numeric_limits<double>::infinity();
	double m_min = std::numeric_limits<double>::infinity();
	double m_sinAcc = 0;
	double m_cosAcc = 0;
	unsigned m_countAcc = 0;
};

class CStatsDisplay : public IObserver<SWeatherInfoType>
{
	void Update(SWeatherInfoType dataType, IObservable<SWeatherInfoType>* observable) override
	{
		auto weatherData = dynamic_cast<CWeatherData*>(observable);
		UpdateIndication(dataType, weatherData);
	}

	void UpdateIndication(SWeatherInfoType dataType, CWeatherData* weatherData)
	{
		switch (static_cast<int>(dataType))
		{
		case 0:
			m_temp.Update(weatherData->GetTemperature());
			m_temp.Print();
			break;
		case 1:
			m_humidity.Update(weatherData->GetHumidity());
			m_humidity.Print();
			break;
		case 2:
			m_pressure.Update(weatherData->GetPressure());
			m_pressure.Print();
			break;
		case 3:
			m_windSpeed.Update(weatherData->GetWindSpeed());
			m_windSpeed.Print();
			break;
		case 4:
			m_windDirection.Update(weatherData->GetWindDirection());
			m_windDirection.Print();
			break;
		default:
			break;
		}
	}

	NormalIndication m_temp = NormalIndication("temp");
	NormalIndication m_humidity = NormalIndication("hum");
	NormalIndication m_pressure = NormalIndication("pressure");
	NormalIndication m_windSpeed = NormalIndication("wind speed");
	CircIndication m_windDirection = CircIndication("wind direction");
};