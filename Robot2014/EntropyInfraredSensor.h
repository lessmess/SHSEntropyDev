#ifndef __ENTROPY_INFRARED_SENSOR__
#define __ENTROPY_INFRARED_SENSOR__

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
#include "EntropyJoystick.h"

class EntropyInfraredSensor : public EntropySubsystemTemplate 
{
	
private:
	
	//Create variables to be referenced internally using class functions.
	AnalogChannel *InfraredSensor;
	
public:
	EntropyInfraredSensor( );
	
	bool Initialize ();
	
	void Cleanup ();
	
	char * GetFeedback();
	
	float GetValue();
	
	float GetAverageValue();
	
	void SetLight(EntropyJoystick * GameStick);

	//Add any specific functions that will be called externally to modify internal variables.
};
#endif
