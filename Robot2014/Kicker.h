#ifndef __KICKER__
#define __KICKER__

#define UNWINDTIME 65
#define PULLTIME 250
#define DISENGAGED 0
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
	float INITIALTIME;
	
	DigitalInput * KickerLatchedSense;
public:
	enum {
		idle,
		pulling,
		unwinding,
		readytoshoot,
		kicked
	};
	
	Kicker( );
	
	int getKickerState() 
	{
		return kickerState;  
	}
	
	bool Initialize ();
	
	void Cleanup ();
	
	void Kick (bool pull, bool kick);
	
	char * GetFeedback();
	
	void Kicker::TestWich( bool RotateOneWay, bool RotateAnotherWay);

};
#endif
