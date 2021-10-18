#pragma once
#include <iostream>

class IDisplay
{
public:
	virtual void UpdateTemp(double temp) = 0;
	virtual void UpdateHumidity(double humidity) = 0;
	virtual void UpdatePressure(double pressure) = 0;
	virtual void UpdateWindSpeed(double windSpeed) = 0;
	virtual void UpdateWindDirection(double windDirection) = 0;
	virtual ~IDisplay() = default;
};

class CDisplay : public IDisplay
{
public:
	void UpdateTemp(double temp) override
	{
		std::cout << "Current temp: " << temp << '\n';
	}
	void UpdateHumidity(double humidity)
	{
		std::cout << "Current humidity: " << humidity << '\n';
	}
	void UpdatePressure(double pressure) override
	{
		std::cout << "Current pressure: " << pressure << '\n';
	}
	void UpdateWindSpeed(double windSpeed) override
	{
		std::cout << "Current wind speed: " << windSpeed << '\n';
	}
	void UpdateWindDirection(double windDirection) override
	{
		std::cout << "Current wind direction: " << windDirection << '\n';
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

class CStatsDisplay : public IDisplay
{
public:
	void UpdateTemp(double temp) override
	{
		m_temp.Update(temp);
		m_temp.Print();
	}
	void UpdateHumidity(double hum) override
	{
		m_hum.Update(hum);
		m_hum.Print();
	}
	void UpdatePressure(double pressure) override
	{
		m_pressure.Update(pressure);
		m_pressure.Print();
	}
	void UpdateWindSpeed(double windSpeed) override
	{
		m_windSpeed.Update(windSpeed);
		m_windSpeed.Print();
	}
	void UpdateWindDirection(double windDirection) override
	{
		m_windDirection.Update(windDirection);
		m_windDirection.Print();
	}
private:
	NormalIndication m_temp = NormalIndication("Temperature");
	NormalIndication m_hum = NormalIndication("Humidity");
	NormalIndication m_pressure = NormalIndication("Pressure");
	NormalIndication m_windSpeed = NormalIndication("Wind speed");
	CircIndication m_windDirection = CircIndication("Wind direction");
};