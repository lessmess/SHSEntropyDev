#ifndef __KICKER__
#define __KICKER__

#define INITIALTIME 50
#define PULLTIME 100

#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
	

class Kicker : public EntropySubsystemTemplate 
{
	
private:
	// The robot shooter left, right, up, down control
	Jaguar *PullMotor;
	Solenoid *Piston_Trigger;
	Solenoid *Piston_Shifter;
	int kickerState;
	int pistonTimer;
	int pullTimer;
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
