#include "Kicker.h"
#include "IODefinitions.h"
#include <iostream>
#include <fstream>
#include <string>

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
	
	INITIALTIME = 50;
	
	pullTimer = PULLTIME;
	unwindTimer = UNWINDTIME;
	KickerLatchedSense = new DigitalInput(IODefinitions::KickerLatchedSense);
	
	typedef struct
	{
		float value;
		string name;
	}valStruct;
	
	valStruct valList[30];
	
	string line;
    ifstream file ("EntropyKickerINI.txt",  std::ifstream::in);
    int index = 0;
    while ( getline (file,line) )
       {
    		
    		char *ptr = strtok((char*)(line.data()), (" ="));
    		while (ptr != NULL && index <= 30)
    		  {
    		    valList[index].name.assign(ptr);
    		    ptr = strtok (NULL, " =");
    		    valList[index].value = atof(ptr);
    		    index++;
    		  }
       }
    file.close();
    
    for(int x = 0;x<30;x++)
    {
    	if (valList[x].name.compare("PullTime")== 0)
    	{	
    		pullTimer = valList[x].value;
    		break;
    	}
    }
    for(int x = 0;x<30;x++)
    {
       	if (valList[x].name.compare("UnwindTime")== 0)
       	{
       		unwindTimer = valList[x].value;
       		break;
       	}
    }
	for(int x = 0;x<30;x++)
    {
       	if (valList[x].name.compare("INITIALTIME")== 0)
       	{
       		INITIALTIME = valList[x].value;
       		break;
       	}
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
	//DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "Kill Switch: %i", KickerLatchedSense->Get());
    //DriverStationLCD::GetInstance()->UpdateLCD();
	
	
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
			if(pullTimer > 0 && (KickerLatchedSense->Get() != LOCKED))
				{
					pullTimer--;
					PullMotor->Set(0.8);
					DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Kicker: %s %d", "Pulling",pullTimer);
					DriverStationLCD::GetInstance()->UpdateLCD();
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
		PullMotor->Set(0.8);
	} 
	else if ( RotateAnotherWay ){
		Piston_Shifter->Set(ENGAGED);
		PullMotor->Set(-0.8);
	}
	else {
		PullMotor->Set(0.0);
	}
}


void Kicker::TestKicker( bool SetLatch, bool SetKick)
{
	if ( SetLatch && SetKick )
		{
			 
		} 
		else if ( SetLatch )
		{   
			Piston_Trigger->Set(LOCKED);
		} 
		else if ( SetKick ){
			Piston_Trigger->Set(RELEASED);
		}
		else 
		{
			
		}

}

void Kicker::TestDisplayLatchSwitch()
{
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "Kill Switch: %i", KickerLatchedSense->Get());
	DriverStationLCD::GetInstance()->UpdateLCD();
}	
	

//DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "Kill Switch: %i", KickerLatchedSense->Get());
    //DriverStationLCD::GetInstance()->UpdateLCD();

char * Kicker::GetFeedback(){ return NULL;}


