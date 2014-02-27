#include "Kicker.h"
#include "IODefinitions.h"

Kicker::Kicker() {
	
	
}

bool Kicker::Initialize()
{
	PullMotor = new Jaguar(IODefinitions::KICKER_PULL); 
	Piston_Trigger = new Solenoid(IODefinitions::KICKER_TRIGGER);
	Piston_Shifter = new Solenoid(IODefinitions::KICKER_SHIFTER);
	Piston_Trigger->Set(LOCKED);
	Piston_Shifter->Set(DISENGAGED);
	kickerState = idle;
	pistonTimer = 0;
	pullTimer = 0;
	unwindTimer = 0;
	KickerLatchedSense = new DigitalInput(IODefinitions::KickerLatchedSense);
	return true;
}

void Kicker::Cleanup()
{ 
	PullMotor->Disable();
	Piston_Trigger->Set(LOCKED);
	Piston_Shifter->Set(DISENGAGED);
}

void Kicker::Kick(bool pull, bool kick)
{
	switch(kickerState)
	{
	    default: 
		case idle: 
			
			if( true == pull)
			{
				PullMotor->Set(0.0);
				kickerState = pulling;
				DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Kicker: %s", "Pulling");
				DriverStationLCD::GetInstance()->UpdateLCD();
				pullTimer = PULLTIME;
				Piston_Shifter->Set(ENGAGED);
			}
			break;
		case pulling:
			if(pullTimer > 0 || (KickerLatchedSense->Get() != LOCKED))
				{
					pullTimer--;
					PullMotor->Set(0.8);
				}
			else// if ((pullTimer <= 0) || (KickerLatchedSense->Get() == LOCKED))
				{
				PullMotor->Set(0.0);
				unwindTimer = UNWINDTIME;
				kickerState = unwinding;
				DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Kicker: %s", "UnWinding");
				DriverStationLCD::GetInstance()->UpdateLCD();
				}
			break;
		case unwinding:
			if(unwindTimer > 0)
			{
				unwindTimer--;
				PullMotor->Set(-0.7);
			}
			else
			{	
				PullMotor->Set(0.0);
				Piston_Shifter->Set(DISENGAGED);
				kickerState = readytoshoot;
				DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Kicker: %s", "ReadyShoot");
				DriverStationLCD::GetInstance()->UpdateLCD();
			}
		case readytoshoot:
			if (true == kick)
			{
				Piston_Trigger->Set(RELEASED);
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
				Piston_Trigger->Set(LOCKED);
				kickerState = idle;
				DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Kicker: %s", "Idle");
				DriverStationLCD::GetInstance()->UpdateLCD();
			}
	}
}

void Kicker::TestWich( bool RotateOneWay, bool RotateAnotherWay )
{
	if ( RotateOneWay && RotateAnotherWay )
	{
		PullMotor->Set(0.0); 
	} 
	else if ( RotateOneWay )
	{   Piston_Shifter->Set(ENGAGED);
		PullMotor->Set(0.5);;
	} 
	else if ( RotateAnotherWay ){
		Piston_Shifter->Set(ENGAGED);
		PullMotor->Set(-0.5);
	}
	else {
		PullMotor->Set(0.0);
	}
}

char * Kicker::GetFeedback(){ return NULL;}


