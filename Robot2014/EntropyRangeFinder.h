
#ifndef __RANGE_FINDER__
#define __RANGE_FINDER__

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
	

class EntropyRangeFinder : public EntropySubsystemTemplate 
{
	
private:
	
	AnalogChannel * AIRangeFinder;
	
public:
	EntropyRangeFinder( );
	
	bool Initialize ();
	
	void Cleanup ();
	
	char * GetFeedback();
	
	float UpdateRangeLine4DS();
	};
#endif

