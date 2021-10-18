#pragma once

#include <boost/signals2.hpp>
#include <map>
#include "Display.h"

enum class SWeatherInfoType
{
	Temperature,
	Humidity,
	Pressure,
	WindSpeed,
	WindDirection
};

class CWeatherData
{
using doubleSignal = boost::signals2::signal<void(double)>;
public:
	void RemoveObserver(SWeatherInfoType type, boost::shared_ptr<IDisplay>& observer)
	{
		m_connections[{type, observer.get()}].disconnect();
	}

	void RegisterObserver(SWeatherInfoType type, boost::shared_ptr<IDisplay>& observer)
	{
		using namespace boost::placeholders;
		switch (static_cast<int>(type))
		{
		case 0:
			m_connections[{type, observer.get()}] = m_tempSignal.connect(
				doubleSignal::slot_type(&IDisplay::UpdateTemp, observer.get(), _1).track(observer)
			);
			break;
		case 1:
			m_connections[{type, observer.get()}] = m_humSignal.connect(
				doubleSignal::slot_type(&IDisplay::UpdateHumidity, observer.get(), _1).track(observer)
			);
			break;
		case 2:
			m_connections[{type, observer.get()}] = m_pressureSignal.connect(
				doubleSignal::slot_type(&IDisplay::UpdatePressure, observer.get(), _1).track(observer)
			);
			break;
		case 3:
			m_connections[{type, observer.get()}] = m_windSpeedSignal.connect(
				doubleSignal::slot_type(&IDisplay::UpdateWindSpeed, observer.get(), _1).track(observer)
			);
			break;
		case 4:
			m_connections[{type, observer.get()}] = m_windDirectionSignal.connect(
				doubleSignal::slot_type(&IDisplay::UpdateWindDirection, observer.get(), _1).track(observer)
			);
			break;
		default:
			break;
		}
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
			m_tempSignal(temp);
		}
	}
	void SetHumidity(double humidity)
	{
		if (m_humidity != humidity)
		{
			m_humidity = humidity;
			m_humSignal(humidity);
		}
	}
	void SetPressure(double pressure)
	{
		if (m_pressure != pressure)
		{
			m_pressure = pressure;
			m_pressureSignal(pressure);
		}
	}
	void SetWindSpeed(double windSpeed)
	{
		if (m_windSpeed != windSpeed)
		{
			m_windSpeed = windSpeed;
			m_windSpeedSignal(windSpeed);
		}
	}
	void SetWindDirection(double windDirection)
	{
		if (m_windDirection != windDirection)
		{
			m_windDirection = windDirection;
			m_windDirectionSignal(windDirection);
		}
	}

private:
	doubleSignal m_tempSignal;
	doubleSignal m_humSignal;
	doubleSignal m_pressureSignal;
	doubleSignal m_windSpeedSignal;
	doubleSignal m_windDirectionSignal;
	std::map<std::pair<SWeatherInfoType, IDisplay*>, boost::signals2::connection> m_connections;
	
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 0.0;
	double m_windSpeed = 0.0;
	double m_windDirection = 0.0;
};