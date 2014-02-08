#ifndef __KICKER__
#define __KICKER__

#define INITIALTIME 50

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
	

class Kicker : public EntropySubsystemTemplate 
{
	
private:
	// The robot shooter left, right, up, down control
	CANJaguar *PullMotor;
	Solenoid *Piston;
	DigitalInput *PullSwitch;
	int kickerState;
	int pistonTimer;
	enum {
		idle,
		pulling,
		readytoshoot,
		kicked
	};


	
public:
	Kicker( );
	
	bool Initialize ();
	
	void Cleanup ();
	
	void Kick (bool pull, bool kick);
	
	char * GetFeedback();

};
#endif
