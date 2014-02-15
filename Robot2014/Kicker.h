#ifndef __KICKER__
#define __KICKER__

#define INITIALTIME 50
#define UNWINDTIME 70
#define PULLTIME 300
#define DISENGAGED 1
#define ENGAGED 0
#define LOCKED 0
#define RELEASED 1


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
	int unwindTimer;
	enum {
		idle,
		pulling,
		unwinding,
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
