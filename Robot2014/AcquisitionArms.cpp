#include "AcquisitionArms.h"
#include "EntropyJoystick.h"
#include "EntropyInfraredSensor.h"
#include "IODefinitions.h"

AcquisitionArms::AcquisitionArms() {

}

bool AcquisitionArms::Initialize() 
{
	airCompressor = new Compressor (pressureSwitchModule, pressureSwitchChannel, compressorRelayModule, compressorRelayChannel);
	upperSolenoid = new Solenoid (upperSolenoidChannel);
	lowerSolenoid = new Solenoid (lowerSolenoidChannel);
	InfraredSensor.Initialize();
	MotorBelt = new Victor(IODefinitions::MOTOR_ACQUISITION_BELT);
	MotorExtender = new Jaguar(IODefinitions::MOTOR_ACQUISITION_EXTENDER);

	return true;
}
void AcquisitionArms::TeleopInitialize()
{
	airCompressor->Start();
	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line1, 1, "Enabled %d", airCompressor->Enabled()); 
	DriverStationLCD::GetInstance()->UpdateLCD();
}
void AcquisitionArms::Cleanup() 
{
	upperSolenoid->Set(false);
	lowerSolenoid->Set(false);
	MotorExtender->Set(0.0);
	MotorBelt->Set(0.0);
	airCompressor->Stop();
}
void AcquisitionArms::UpperVerticalPos(EntropyJoystick * GameStick) 
{
	if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_UP) && GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_DOWN))
	{}
	else if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_UP)) {
		upperSolenoid->Set(true);
	} else if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_DOWN)) {
		upperSolenoid->Set(false);
	}
	
	//bool state1 = GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_DOWN) && !GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_UP);
}
void AcquisitionArms::LowerVerticalPos(EntropyJoystick * GameStick)
{
	lowerSolenoid->Set(GameStick->GetRawButton(IODefinitions::KICKER_TRIGGERKICK));
}
void AcquisitionArms::Update()
{
	if (InfraredSensor.GetValue() > 1.4)
	{
		lowerSolenoid->Set(true);
	} else {
		lowerSolenoid->Set(false);
	}
}
void AcquisitionArms::Extend(EntropyJoystick * GameStick)
{
	if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_OUT)) {
		MotorExtender->Set(0.5);
	} else if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_IN)) {
		MotorExtender->Set(-0.5);
	} else {
		MotorExtender->Set(0.0);
	}
}
void AcquisitionArms::BeltEnable(EntropyJoystick * GameStick)
{
	if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_UP)) {
		MotorBelt->Set(0.2);
	} else if (GameStick->GetRawButton(IODefinitions::GAME_BUTTON_ARM_DOWN)){
		MotorBelt->Set(-0.2);
	} else {
		MotorBelt->Set(0.0);
	}
}

char * AcquisitionArms::GetFeedback(){ return NULL;}
