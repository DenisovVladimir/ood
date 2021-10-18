#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"
#include "../Task8/WeatherData.h"
#include <sstream>

using namespace std;

class CTestDisplay : public IDisplay
{
public:
	CTestDisplay(ostream& stream)
		:m_stream(stream) {}
	void UpdateTemp(double temp) override
	{
		m_stream << temp << '\n';
	}
	void UpdateHumidity(double humidity) override
	{
		m_stream << humidity << '\n';
	}
	void UpdatePressure(double pressure) override
	{
		m_stream << pressure << '\n';
	}
	void UpdateWindSpeed(double windSpeed) override
	{
		m_stream << windSpeed << '\n';
	}
	void UpdateWindDirection(double windDirection) override
	{
		m_stream << windDirection << '\n';
	}
private:
	ostream& m_stream;
};

SCENARIO("Test weather data")
{
	GIVEN("1 weather data and test display")
	{
		stringstream ss;
		string str;
		boost::shared_ptr<IDisplay> testDisplay(new CTestDisplay(ss));
		CWeatherData wd1;
		WHEN("Observe temperature")
		{
			wd1.RegisterObserver(SWeatherInfoType::Temperature, testDisplay);
			wd1.SetMeasurements(15, 60, 760, 8, 100);
			getline(ss, str);
			REQUIRE(str == "15");
			wd1.RemoveObserver(SWeatherInfoType::Temperature, testDisplay);
			wd1.SetMeasurements(20, 70, 761, 7, 150);
			getline(ss, str);
			REQUIRE(str == "");
		}
		WHEN("Observe humididty")
		{
			wd1.RegisterObserver(SWeatherInfoType::Humidity, testDisplay);
			wd1.SetMeasurements(15, 60, 760, 8, 100);
			getline(ss, str);
			REQUIRE(str == "60");
			wd1.RemoveObserver(SWeatherInfoType::Humidity, testDisplay);
			wd1.SetMeasurements(20, 70, 761, 7, 150);
			getline(ss, str);
			REQUIRE(str == "");
		}
		WHEN("Observe pressure")
		{
			wd1.RegisterObserver(SWeatherInfoType::Pressure, testDisplay);
			wd1.SetMeasurements(15, 60, 760, 8, 100);
			getline(ss, str);
			REQUIRE(str == "760");
			wd1.RemoveObserver(SWeatherInfoType::Pressure, testDisplay);
			wd1.SetMeasurements(20, 70, 761, 7, 150);
			getline(ss, str);
			REQUIRE(str == "");
		}
		WHEN("Observe wind speed")
		{
			wd1.RegisterObserver(SWeatherInfoType::WindSpeed, testDisplay);
			wd1.SetMeasurements(15, 60, 760, 8, 100);
			getline(ss, str);
			REQUIRE(str == "8");
			wd1.RemoveObserver(SWeatherInfoType::WindSpeed, testDisplay);
			wd1.SetMeasurements(20, 70, 761, 7, 150);
			getline(ss, str);
			REQUIRE(str == "");
		}
		WHEN("Observe wind direction")
		{
			wd1.RegisterObserver(SWeatherInfoType::WindDirection, testDisplay);
			wd1.SetMeasurements(15, 60, 760, 8, 100);
			getline(ss, str);
			REQUIRE(str == "100");
			wd1.RemoveObserver(SWeatherInfoType::WindDirection, testDisplay);
			wd1.SetMeasurements(20, 70, 761, 7, 150);
			getline(ss, str);
			REQUIRE(str == "");
		}
	}

	GIVEN("2 weather data and test diplay")
	{
		stringstream ss;
		string str;
		boost::shared_ptr<IDisplay> testDisplay(new CTestDisplay(ss));
		CWeatherData wd1;
		CWeatherData wd2;

		THEN("")
		{
			wd1.RegisterObserver(SWeatherInfoType::Temperature, testDisplay);
			wd2.RegisterObserver(SWeatherInfoType::Pressure, testDisplay);
			wd1.SetMeasurements(10, 80, 760, 8, 200);
			wd2.SetMeasurements(20, 75, 762, 9, 180);
			getline(ss, str);
			REQUIRE(str == "10");
			getline(ss, str);
			REQUIRE(str == "762");
		}
	}
}