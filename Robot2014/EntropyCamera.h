
#ifndef __ENTROPY_CAMERA__
#define __ENTROPY_CAMERA__

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
	

class EntropyCamera : public EntropySubsystemTemplate 
{
	
private:
	
	Servo * CameraServo;
		
public:
	EntropyCamera( );
	
	bool Initialize ();
	
	void Cleanup ();
	
	char * GetFeedback();
	
	void SetCameraPositionTelop();
	
	void SetCameraPositionAuto();
	

	};
#endif

