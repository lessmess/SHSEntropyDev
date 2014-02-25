
#include "EntropyInfraredSensor.h"
#include "IODefinitions.h"



EntropyInfraredSensor::EntropyInfraredSensor() {
	
	AIIFSensor = new AnalogChannel (IODefinitions::BALL_POSESSION_DETECTOR);
}

bool EntropyInfraredSensor::Initialize()
{
	
	return true;
}


float EntropyInfraredSensor::UpdateRangeLine5DS()
{
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "Ball: %f", AIIFSensor->GetVoltage());
	DriverStationLCD::GetInstance()->UpdateLCD();
	return 0.0;
}	
	
void EntropyInfraredSensor::Cleanup()
{ 
	
}

char * EntropyInfraredSensor::GetFeedback(){ return NULL;}

