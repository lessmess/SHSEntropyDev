#ifndef __ACQUISITION_ARMS__
#define __ACQUISITION_ARMS__

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
#include "EntropyJoystick.h"
#include "EntropyInfraredSensor.h"
#include "IODefinitions.h"

class AcquisitionArms : public EntropySubsystemTemplate
{
private:
	static const int upperSolenoidChannel = IODefinitions::ARM_ROTATION;
	static const int lowerSolenoidChannel = IODefinitions::ARM_RAIL;
	Solenoid * upperSolenoid;
	Solenoid * lowerSolenoid;
	
	EntropyInfraredSensor InfraredSensor;
	Victor * MotorBelt;
	Jaguar * MotorExtender;
	Solenoid * CradleLEDs;
	
	bool ArmUp,CradleUp;

public:
	AcquisitionArms();
	
	bool Initialize ();
	
	void SetAutoInitialState();
	
	void SetTelopInitialState();
		
	void Cleanup();
		
	char * GetFeedback();
	
	void UpperVerticalPos(bool Arm_Up, bool Arm_Down);
	
	void LowerVerticalPos(bool Cradle_Up, bool Cradle_Down);

	void Extend(bool Arm_Out, bool Arm_In, bool Arm_Down);
	
	void BeltEnable(bool Roll_In, bool Roll_Out);
	
	bool IsArmUp(){ return ArmUp; }
	
	bool IsCradleUp() { return CradleUp; } 
	
};

#endif
