
#include "EntropyCamera.h"
#include "IODefinitions.h"


EntropyCamera::EntropyCamera() {
	
	CameraServo = new Servo(IODefinitions::PWM_CAMERA_CONTROL);
	
}

bool EntropyCamera::Initialize()
{
	
		return true;
}


void EntropyCamera::SetCameraPositionTelop()
{
	CameraServo->SetAngle(100);
}	
	
void EntropyCamera::SetCameraPositionAuto()
{
	CameraServo->SetAngle(65);
}


void EntropyCamera::Cleanup()
{ 
	
}

char * EntropyCamera::GetFeedback(){ return NULL;}


