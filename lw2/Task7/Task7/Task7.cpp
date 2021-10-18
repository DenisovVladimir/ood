#include <iostream>
#include "WeatherStation.h"

using namespace std;

int main()
{
	CWeatherData wd;
	CDisplay display;
	CStatsDisplay sDisplay;
	wd.RegisterObserver(SWeatherInfoType::Temperature, display);
	wd.RegisterObserver(SWeatherInfoType::Pressure, display);
	wd.RegisterObserver(SWeatherInfoType::Temperature, sDisplay);
	wd.RegisterObserver(SWeatherInfoType::Pressure, sDisplay);


	wd.SetMeasurements(-15, 75, 760, 8, 50);
	cout << "----------\n";
	wd.SetMeasurements(-17, 74, 762, 8, 61);
	cout << "----------\n";
	wd.SetMeasurements(-17, 73, 759, 7, 46);
}