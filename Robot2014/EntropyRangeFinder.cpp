
#include "EntropyRangeFinder.h"
#include "IODefinitions.h"
#include "math.h"


EntropyRangeFinder::EntropyRangeFinder() {
	
	AIRangeFinder = new AnalogChannel (IODefinitions::RANGE_FINDER);
}

bool EntropyRangeFinder::Initialize()
{
	
		return true;
}


float EntropyRangeFinder::UpdateRangeLine4DS()
{
	float RangeFT;
		
	//return voltage until formula is in place
	//Read value in Volts, Sensor is 9.8mV/inch. Raneg converter to ft
	RangeFT = (AIRangeFinder->GetVoltage())*1000/9.8/12 ;
		
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line4, "Range: %4.2f ft", RangeFT);
	DriverStationLCD::GetInstance()->UpdateLCD();
	return RangeFT;
}	
	
void EntropyRangeFinder::Cleanup()
{ 
	
}

char * EntropyRangeFinder::GetFeedback(){ return NULL;}


