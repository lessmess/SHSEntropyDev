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
	static const int compressorRelayChannel = IODefinitions::COMPRESSOR_RELAY_CHANNEL;
	static const int pressureSwitchChannel = IODefinitions::PRESSURE_SWITCH_CHANNEL;
	static const int compressorRelayModule = IODefinitions::COMPRESSOR_RELAY_MODULE;
	static const int pressureSwitchModule = IODefinitions::PRESSURE_SWITCH_MODULE;
	static const int upperSolenoidChannel = IODefinitions::ARM_ROTATION;
	static const int lowerSolenoidChannel = IODefinitions::ARM_RAIL;
	Compressor * airCompressor;
	Solenoid * upperSolenoid;
	Solenoid * lowerSolenoid;
	
	EntropyInfraredSensor InfraredSensor;
	Victor * MotorBelt;
	Jaguar * MotorExtender;
public:
	AcquisitionArms();
	
	bool Initialize ();
	
	void Update();
	
	void TeleopInitialize();
		
	void Cleanup();
		
	char * GetFeedback();
	
	void UpperVerticalPos(EntropyJoystick * GameStick);
	
	void LowerVerticalPos(EntropyJoystick * GameStick);

	void Extend(EntropyJoystick * GameStick);
	
	void BeltEnable(EntropyJoystick * GameStick);
};

#endif
