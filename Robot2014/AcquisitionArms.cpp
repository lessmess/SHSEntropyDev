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
	InfraredSensor.Initialize();
	MotorBelt = new Victor(IODefinitions::MOTOR_ACQUISITION_BELT);
	MotorExtender = new Jaguar(IODefinitions::MOTOR_ACQUISITION_EXTENDER);
	ArmUp = true; 
	CradleUp = false;

	return true;
}
void AcquisitionArms::SetAutoInitialState()
{
	CradleUp = true;
	lowerSolenoid->Set(1);
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
void AcquisitionArms::UpperVerticalPos(EntropyJoystick * GameStick) 
{
	if( GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_UP) 
			^ GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_DOWN))
	{
		upperSolenoid->Set(GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_UP));
		ArmUp = (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_UP) != 1);
	}
	
}
void AcquisitionArms::LowerVerticalPos(EntropyJoystick * GameStick)
{
	if( GameStick->GetRawButton(IODefinitions::GAME_BUTTON_CRADLE_DOWN) 
			^ GameStick->GetRawButton(IODefinitions::GAME_BUTTON_CRADLE_UP))
	{
		lowerSolenoid->Set(GameStick->GetRawButton(IODefinitions::GAME_BUTTON_CRADLE_UP));
		CradleUp = (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_CRADLE_UP) == 1);
	}
}

void AcquisitionArms::Extend(EntropyJoystick * GameStick)
{
	if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_OUT)) {
		 extendCounter=0;
		MotorExtender->Set(0.75);
	} else if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_IN)) {
		 extendCounter=0;
		MotorExtender->Set(-0.75);
	} else if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_DOWN)
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

void AcquisitionArms::BeltEnable(EntropyJoystick * GameStick)
{
	if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_ROLL_IN)) {
		MotorBelt->Set(1.0);
	} else if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_ROLL_OUT)){
		MotorBelt->Set(-1.0);
	} else {
		MotorBelt->Set(0.0);
	}
}

char * AcquisitionArms::GetFeedback(){ return NULL;}
