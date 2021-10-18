#pragma once

#include "Observer.h"

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
};

struct SWeatherInfoPro : public SWeatherInfo
{
	double windSpeed = 0;
	double windDirection = 0;
};


template<typename T>
class CWeaterDataBase : public CObservable<T>
{
public:
	void MeasurementsChanged()
	{
		CObservable<T>::NotifyObservers();
	}

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
protected:

	void SetTemperature(double temp)
	{
		m_temperature = temp;
	}
	void SetHumidity(double hum)
	{
		m_humidity = hum;
	}
	void SetPressure(double pressure)
	{
		m_pressure = pressure;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
};

class CWeatherData : public CWeaterDataBase<SWeatherInfo>
{
public:
	void SetMeasurements(double temp, double humidity, double pressure)
	{
		SetTemperature(temp);
		SetHumidity(humidity);
		SetPressure(pressure);

		MeasurementsChanged();
	}
protected:
	SWeatherInfo GetChangedData() const override
	{
		SWeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		return info;
	}
};

class CWeatherDataPro : public CWeaterDataBase<SWeatherInfoPro>
{
public:
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
		SetTemperature(temp);
		SetHumidity(humidity);
		SetPressure(pressure);
		m_windSpeed = windSpeed;
		m_windDirection = windDirection;

		MeasurementsChanged();
	}
protected:
	SWeatherInfoPro GetChangedData() const override
	{
		SWeatherInfoPro info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		info.windSpeed = GetWindSpeed();
		info.windDirection = GetWindDirection();
		return info;
	}
private:
	double m_windSpeed = 0;
	double m_windDirection = 0;
};