
#ifndef __INFRARED_SENSOR__
#define __INFRARED_SENSOR__

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
	

class EntropyInfraredSensor : public EntropySubsystemTemplate 
{
	
private:
	
	AnalogChannel * AIIFSensor;
	
public:
	EntropyInfraredSensor( );
	
	bool Initialize ();
	
	void Cleanup ();
	
	char * GetFeedback();
	
	float UpdateRangeLine5DS();
	};
#endif
