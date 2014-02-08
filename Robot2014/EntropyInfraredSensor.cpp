#include "EntropyInfraredSensor.h"
#include "IODefinitions.h"
#include "EntropyJoystick.h"

EntropyInfraredSensor::EntropyInfraredSensor() {
	
	
}

bool EntropyInfraredSensor::Initialize()    //Create necessary variables by connecting them to specific IO definitions.
{
	
	InfraredSensor = new AnalogChannel(IODefinitions::AUTONOMOUS_SELECTOR_SWITCH);
	return true;
}

void EntropyInfraredSensor::Cleanup()
{ 
	//Analog Channel does not support any disable or kill commands
	// to kill a variable, VARNAME->Disable();
}


float EntropyInfraredSensor::GetValue()
{
	return InfraredSensor->GetVoltage();
}

float EntropyInfraredSensor::GetAverageValue()
{
	return InfraredSensor->GetAverageVoltage();
}

//Add additional functions to be more specific from what was defined in the .h file.

char * EntropyInfraredSensor::GetFeedback(){ return NULL;}


