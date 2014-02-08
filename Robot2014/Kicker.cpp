#include "Kicker.h"
#include "IODefinitions.h"

Kicker::Kicker() {
	
	
}

bool Kicker::Initialize()
{
	//TODO: FIND PORTS FOR THESE
	PullMotor = new CANJaguar(IODefinitions::KICKER_PULL_CAN_JAG); 
	Piston = new Solenoid(IODefinitions::KICKER_SOLONOID);
	PullSwitch = new DigitalInput(IODefinitions::PULL_SWITCH_DIGITAL_CHANNEL, 
			IODefinitions::PULL_SWITCH_DIGITAL_MODULE);
	Piston->Set(0);
	kickerState = idle;
	pistonTimer = 0;
	return true;
}

void Kicker::Cleanup()
{ 
	PullMotor->Disable();
	Piston->Set(0);
}

void Kicker::Kick(bool pull, bool kick)
{
	switch(kickerState)
	{
	    default: 
		case idle: 
			if( true == pull)
			{
				PullMotor->Set(0.1);
				kickerState = pulling;
			}
			break;
		case pulling:
			if (true == PullSwitch->Get())
			{
				PullMotor->Set(0.0);
				kickerState = readytoshoot;
			}	
			
			break;
		case readytoshoot:
			if (true == kick)
			{
				Piston->Set(0);
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
				Piston->Set(1);
				kickerState = idle;
			}
	}
}

char * Kicker::GetFeedback(){ return NULL;}


