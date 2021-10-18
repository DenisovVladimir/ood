#include <iostream>
#include "WeatherData.h"

using namespace std;

int main()
{
	boost::shared_ptr<IDisplay> display(new CDisplay);
	boost::shared_ptr<IDisplay> sdisplay(new CStatsDisplay);
	CWeatherData wd1;
	CWeatherData wd2;
	wd1.RegisterObserver(SWeatherInfoType::Temperature, display);
	wd1.RegisterObserver(SWeatherInfoType::Humidity, sdisplay);
	wd2.RegisterObserver(SWeatherInfoType::Temperature, display);

	wd1.SetMeasurements(15, 75, 760, 7, 260);
	wd2.SetMeasurements(20, 70, 762, 10, 9);

	wd2.RemoveObserver(SWeatherInfoType::Temperature, display);
	wd2.SetMeasurements(30, 70, 762, 10, 9);
}
