#include "AcquisitionArms.h"
#include "EntropyJoystick.h"
#include "EntropyInfraredSensor.h"
#include "IODefinitions.h"

const double seconds=1.4;
const double extendCounterStart=(seconds*1000)/50;
double extendCounter;


AcquisitionArms::AcquisitionArms() {
	
}

bool AcquisitionArms::Initialize() 
{
	upperSolenoid = new Solenoid (upperSolenoidChannel);
	lowerSolenoid = new Solenoid (lowerSolenoidChannel);
	CradleLEDs = new Solenoid (IODefinitions::CRADLE_LEDS);
	InfraredSensor.Initialize();
	MotorBelt = new Victor(IODefinitions::MOTOR_ACQUISITION_BELT);
	MotorExtender = new Jaguar(IODefinitions::MOTOR_ACQUISITION_EXTENDER);
	
	return true;
}

void AcquisitionArms::SetAutoInitialState()
{
	CradleUp = true;
	lowerSolenoid->Set(1);
}

void AcquisitionArms::SetTelopInitialState()
{
	ArmUp = true; 
	CradleUp = false;
 
}

void AcquisitionArms::Cleanup() 
{
	ArmUp = false; 
	CradleUp = false;
	upperSolenoid->Set(false);
	lowerSolenoid->Set(false);
	MotorExtender->Set(0.0);
	MotorBelt->Set(0.0);
	
}
void AcquisitionArms::UpperVerticalPos(bool Arm_Up, bool Arm_Down) 
{
	if( Arm_Up 
			^ Arm_Down)
	{
		upperSolenoid->Set(Arm_Up);
		ArmUp = (Arm_Up != 1);
		
	}
	
	
}
void AcquisitionArms::LowerVerticalPos(bool Cradle_Up, bool Cradle_Down)
{
	if( Cradle_Down 
			^ Cradle_Up)
	{
		lowerSolenoid->Set(Cradle_Up);
		CradleUp = (Cradle_Up == 1);
		CradleLEDs->Set(CradleUp);
	}
}

void AcquisitionArms::Extend(bool Arm_Out, bool Arm_In, bool Arm_Down)
{
	if (Arm_Out) {
		 extendCounter=0;
		MotorExtender->Set(0.75);
	} else if (Arm_In) {
		 extendCounter=0;
		MotorExtender->Set(-0.75);
	} else if (Arm_Down
			and extendCounter==0){
		extendCounter=extendCounterStart;
		MotorExtender->Set(-0.75);
	} else if (extendCounter>0){
		extendCounter--;
		MotorExtender->Set(-0.75);
	} else {
		MotorExtender->Set(0.0);
	}
}

void AcquisitionArms::BeltEnable(bool Roll_In, bool Roll_Out)
{
	if (Roll_In) {
		MotorBelt->Set(1.0);
	} else if (Roll_Out){
		MotorBelt->Set(-1.0);
	} else {
		MotorBelt->Set(0.0);
	}
}

char * AcquisitionArms::GetFeedback(){ return NULL;}
