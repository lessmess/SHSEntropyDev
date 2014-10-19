#ifndef __CONTROLLER_COMMANDS__
#define __CONTROLLER_COMMANDS__

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
#include "EntropyJoystick.h"
#include "EntropyInfraredSensor.h"
#include "IODefinitions.h"
#include "AcquisitionArms.h"
#include "Kicker.h"



class EntropyGamepad : public EntropySubsystemTemplate
{
	
	AcquisitionArms *MyArms;
	Kicker *MyKicker;
private:
	

public:
	double kick_counter; 
	
	EntropyGamepad();
	
	void Human_Aquire();
	
	void Field_Aquire();
	
	void Object_Transport();
	
	void Shoot_High();
	
	void Shoot_Low();
	
	
	bool Initialize();
	void Cleanup();
	char * GetFeedback();
};

#endif
