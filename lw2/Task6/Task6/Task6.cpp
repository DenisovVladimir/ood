#include "Displays.h"
#include "WeatherStations.h"

using namespace std;

int main()
{
	CWeatherData wdIn;
	CWeatherDataPro wdOut;
	
	CWeatherStation station;
	station.RegisterInStation(wdIn);
	station.RegisterOutStation(wdOut);

	wdIn.SetMeasurements(15, 85, 760);
	wdOut.SetMeasurements(16, 76, 762, 8, 0);
	wdOut.SetMeasurements(18, 84, 760, 8, 0);
	wdOut.SetMeasurements(14, 80, 761, 8, 90);
	
	return 0;
}