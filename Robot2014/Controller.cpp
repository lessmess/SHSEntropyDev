#include "Controller.h"

EntropyGamepad::EntropyGamepad()
{
	MyArms = new AcquisitionArms;
	MyKicker = new Kicker;
}

void EntropyGamepad::Human_Aquire()
{
	
	MyArms->UpperVerticalPos(true, false);
	MyArms->Extend(false, true, false);
	MyArms->LowerVerticalPos(true, false);
	MyKicker->Kick(true, false);
}

void EntropyGamepad::Field_Aquire()
{
	MyArms->UpperVerticalPos(false, true);
	MyArms->Extend(false, true, false);
	MyArms->BeltEnable(true, false);
	MyKicker->Kick(true, false);
	
}

void EntropyGamepad::Object_Transport() //This is the default state
{
	MyArms->UpperVerticalPos(false, true);
	MyArms->Extend(false, true, false);
	MyArms->LowerVerticalPos(true, false);
	MyArms->BeltEnable(false, false);
	
}

void EntropyGamepad::Shoot_High()
{
	
	MyArms->UpperVerticalPos(true, false);
	MyArms->Extend(false, true, false);
	MyArms->LowerVerticalPos(true, false);
	
	if (GetClock() - kick_counter > 4.0)
	{
		if (MyKicker->getKickerState() == Kicker::readytoshoot)
		{
			MyKicker->Kick(false, true);
		}
		
	}
}

void EntropyGamepad::Shoot_Low()
{
	MyArms->UpperVerticalPos(false, true);
	MyArms->Extend(false, true, false);
	MyArms->BeltEnable(false, true);
	
}

bool EntropyGamepad::Initialize(){}
void EntropyGamepad::Cleanup(){}
char * EntropyGamepad::GetFeedback(){}

