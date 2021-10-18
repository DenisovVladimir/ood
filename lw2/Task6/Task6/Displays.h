#pragma once
#include <iostream>

#include "WeatherStations.h"
#include "Observer.h"

template<typename T>
class CDisplayBase : public IObserver<T>
{
protected:
	CDisplayBase(const std::string locationName)
		:m_locationName(locationName) {}
	void PrintInfo(const std::string& name, double data)
	{
		std::cout << m_locationName << ' ' << name << ' ' << data << '\n';
	}
private:
	std::string m_locationName;
};

class CDisplay : public CDisplayBase<SWeatherInfo>
{
public:
	CDisplay(const std::string locationName)
		:CDisplayBase(locationName) {}
protected:
	void Update(SWeatherInfo const& data) override
	{
		PrintInfo("temp", data.temperature);
		PrintInfo("hum", data.humidity);
		PrintInfo("pressure", data.pressure);
		std::cout << "----------------\n";
	}
};

class CDisplayPro : public CDisplayBase<SWeatherInfoPro>
{
public:
	CDisplayPro(const std::string& locationName)
		:CDisplayBase(locationName) {}
protected:
	void Update(SWeatherInfoPro const& data) override
	{
		PrintInfo("temp", data.temperature);
		PrintInfo("hum", data.humidity);
		PrintInfo("pressure", data.pressure);
		PrintInfo("wind speed", data.windSpeed);
		PrintInfo("wind direction", data.windDirection);
		std::cout << "----------------\n";
	}
};

class Indication
{
public:
	Indication(const std::string& name)
		:m_name(name) {}

	virtual void Update(double newData) = 0;
	virtual void Print(const std::string& location) const = 0;
protected:
	std::string m_name;
};

class NormalIndication : public Indication
{
public:
	NormalIndication(const std::string& inputName)
		:Indication(inputName) {}

	void Update(double newData) override
	{
		m_max = std::max(newData, m_max);
		m_min = std::min(newData, m_min);
		m_acc += newData;
		++m_countAcc;
	}
	void Print(const std::string& location) const override
	{
		std::cout << "Max " << location << ' ' << m_name << ' ' << m_max << '\n';
		std::cout << "Min " << location << ' ' << m_name << ' ' << m_min << '\n';
		std::cout << "Average " << location << ' ' << m_name << ' ' << (m_acc / m_countAcc) << '\n';
	}
private:
	double m_max = -std::numeric_limits<double>::infinity();
	double m_min = std::numeric_limits<double>::infinity();
	double m_acc = 0;
	unsigned m_countAcc = 0;
};

struct CircIndication : public Indication
{
public:
	CircIndication(const std::string& inputName)
		:Indication(inputName) {}

	const double PI = std::atan(1) * 4;
	void Update(double newData) override
	{
		m_max = std::max(newData, m_max);
		m_min = std::min(newData, m_min);
		++m_countAcc;
		m_sinAcc += std::sin(newData * PI / 180);
		m_cosAcc += std::cos(newData * PI / 180);
	}

	void Print(const std::string& location) const override
	{
		std::cout << "Max " << location << ' ' << m_name << ' ' << m_max << '\n';
		std::cout << "Min " << location << ' ' << m_name << ' ' << m_min << '\n';
		std::cout << "Average " << location << ' ' << m_name << ' ' 
			<< (std::atan2(m_sinAcc / m_countAcc, m_cosAcc / m_countAcc) * 180 / PI) << '\n';
	}
private:
	double m_max = -std::numeric_limits<double>::infinity();
	double m_min = std::numeric_limits<double>::infinity();
	double m_sinAcc = 0;
	double m_cosAcc = 0;
	unsigned m_countAcc = 0;
};


class CStatsDisplay : public IObserver<SWeatherInfo>
{
public:
	CStatsDisplay(const std::string& locationName)
		:m_locationName(locationName) {}
protected:
	void Update(SWeatherInfo const& data) override
	{
		m_temp.Update(data.temperature);
		m_hum.Update(data.humidity);
		m_pressure.Update(data.pressure);

		m_temp.Print(m_locationName);
		m_hum.Print(m_locationName);
		m_pressure.Print(m_locationName);
		std::cout << "----------------\n";
	}
private:
	std::string m_locationName;
	NormalIndication m_temp = NormalIndication("Temp");
	NormalIndication m_hum = NormalIndication("Hum");
	NormalIndication m_pressure = NormalIndication("Pressure");
};

class CStatsDisplayPro : public IObserver<SWeatherInfoPro>
{
public:
	CStatsDisplayPro(const std::string& locationName)
		:m_locationName(locationName) {}
protected:
	void Update(SWeatherInfoPro const& data) override
	{
		m_temp.Update(data.temperature);
		m_hum.Update(data.humidity);
		m_pressure.Update(data.pressure);
		m_windSpeed.Update(data.windSpeed);
		m_windDirection.Update(data.windDirection);

		m_temp.Print(m_locationName);
		m_hum.Print(m_locationName);
		m_pressure.Print(m_locationName);
		m_windSpeed.Print(m_locationName);
		m_windDirection.Print(m_locationName);
		std::cout << "----------------\n";
	}
private:
	std::string m_locationName;
	NormalIndication m_temp = NormalIndication("Temp");
	NormalIndication m_hum = NormalIndication("Hum");
	NormalIndication m_pressure = NormalIndication("Pressure");
	NormalIndication m_windSpeed = NormalIndication("Wind Speed");
	CircIndication m_windDirection = CircIndication("Wind Direction");
};

class CWeatherStation
{
public:
	void RegisterInStation(IObservable<SWeatherInfo>& observable)
	{
		observable.RegisterObserver(m_inDisplay, 500);
		observable.RegisterObserver(m_inStatsDisplay, 450);
	}

	void RegisterOutStation(IObservable<SWeatherInfoPro>& observable)
	{
		observable.RegisterObserver(m_outDisplay, 250);
		observable.RegisterObserver(m_outStatsDisplay, 200);
	}
private:
	CDisplay m_inDisplay = CDisplay("Inside");
	CDisplayPro m_outDisplay = CDisplayPro("Outside");
	CStatsDisplay m_inStatsDisplay = CStatsDisplay("Inside");
	CStatsDisplayPro m_outStatsDisplay = CStatsDisplayPro("Outside");
};