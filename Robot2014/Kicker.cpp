#include "Kicker.h"
#include "IODefinitions.h"

Kicker::Kicker() {
	
	
}

bool Kicker::Initialize()
{
	PullMotor = new Jaguar(IODefinitions::KICKER_PULL); 
	Piston_Trigger = new Solenoid(IODefinitions::KICKER_TRIGGER);
	Piston_Shifter = new Solenoid(IODefinitions::KICKER_SHIFTER);
	Piston_Trigger->Set(0);
	Piston_Shifter->Set(0);
	kickerState = idle;
	pistonTimer = 0;
	pullTimer = 0;
	return true;
}

void Kicker::Cleanup()
{ 
	PullMotor->Disable();
	Piston_Trigger->Set(0);
}

void Kicker::Kick(bool pull, bool kick)
{
	switch(kickerState)
	{
	    default: 
		case idle: 
			if( true == pull)
			{
				Piston_Shifter->Set(1);
				PullMotor->Set(0.5);
				kickerState = pulling;
				pullTimer = PULLTIME;
			}
			break;
		case pulling:
			if(pullTimer > 0)
				{
					pullTimer--;
				}
			else
				{
				PullMotor->Set(0.0);
				Piston_Shifter->Set(0);
				kickerState = readytoshoot;
				}
			break;
		case readytoshoot:
			if (true == kick)
			{
				Piston_Trigger->Set(0);
				pistonTimer = INITIALTIME;
				kickerState = kicked;
			}
			break;
		case kicked:
			if(pistonTimer > 0)
			{
				pistonTimer--;
			}
			else
			{
				Piston_Trigger->Set(1);
				kickerState = idle;
			}
	}
}

char * Kicker::GetFeedback(){ return NULL;}


