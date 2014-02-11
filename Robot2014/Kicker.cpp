#include "Kicker.h"
#include "IODefinitions.h"
#include <string>
#include <iostream>
#include <fstream>

#define NO_OF_KEYS 2

string keys[NO_OF_KEYS] = {
			"time",
			"speed"
	};
	
	float vals[NO_OF_KEYS];

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
	
	string line;
	ifstream file("kickerinit.txt");
	
	if (NULL != file)
	{
		while (getline(file,line))
		{
			bool key_found = false;
			int x;
			for (x = 0; x>NO_OF_KEYS; x++)
			{
				if (string::npos != line.find(keys[x]))
				{
					key_found = true;
					break;
				}
			}
			if (key_found == true)
			{
				char * cptr = (char*)line.data();
				vals[x] = atof(strtok(cptr, "= "));
			}
		}
		file.close();
	}
	
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
				Piston_Shifter->Set(ENGAGED);
				PullMotor->Set(0.4);
				kickerState = pulling;
				for (int x = 0; x>NO_OF_KEYS; x++){
					if (keys[x] == "time"){
						pullTimer = (int)vals[x];
					}
				}
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
				Piston_Shifter->Set(DISENGAGED);
				kickerState = readytoshoot;
				}
			break;
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
			}
	}
}

char * Kicker::GetFeedback(){ return NULL;}


