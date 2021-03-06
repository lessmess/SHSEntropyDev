#include "IODefinitions.h"
#include "WPILib.h"
#include "EntropySubsystemTemplate.h"
#include "EntropyDrive.h"
#include "EntropyDriveTable.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#define DEADZONE 1


double DEAD_ZONE_MAX = .15;

float CompMoveValuePlus=0.99;
float CompMoveValueMinus=0.60;
float CompRotateValuePlus=0.99;
float CompRotateValueMinus=0.99;

double dampValue=0.05;


bool EntropyDrive::Initialize () 
{ 
	MotorDriveLeft1 = new CANJaguar(IODefinitions::MOTOR_DRIVE_LEFT_1);
	MotorDriveLeft2 = new CANJaguar(IODefinitions::MOTOR_DRIVE_LEFT_2);
	MotorDriveRight1 = new CANJaguar(IODefinitions::MOTOR_DRIVE_RIGHT_1);
	MotorDriveRight2 = new CANJaguar(IODefinitions::MOTOR_DRIVE_RIGHT_2);

	wpiDrive = new RobotDrive( 	MotorDriveLeft1,
			MotorDriveLeft2,
			MotorDriveRight1,
			MotorDriveRight2 );
	
	typedef struct
	{
		float value;
		string name;
	}valStruct;
	
	valStruct valList[30];
	
	string line;
    ifstream file ("EntropyDriveINI.txt",  std::ifstream::in);
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
    	if (valList[x].name.compare("DEAD_ZONE_MAX")== 0)
    	{	
    		DEAD_ZONE_MAX = valList[x].value;
    		break;
    	}
    }
    for(int x = 0;x<30;x++)
	{
        if (valList[x].name.compare("CompMoveValuePlus")== 0)
        {
        	CompMoveValuePlus = valList[x].value;
        	break;
        }
    }
    for(int x = 0;x<30;x++)
    {
       	if (valList[x].name.compare("CompMoveValueMinus")== 0)
       	{
       		CompMoveValueMinus = valList[x].value;
       		break;
       	}
    }
	for(int x = 0;x<30;x++)
    {
       	if (valList[x].name.compare("CompRotateValuePlus")== 0)
       	{
       		CompRotateValuePlus = valList[x].value;
       		break;
       	}
    }
	for(int x = 0;x<30;x++)
    {
       	if (valList[x].name.compare("CompRotateValueMinus")== 0)
       	{
       		CompRotateValueMinus = valList[x].value;
       		break;
       	}
    }
	for(int x = 0;x<30;x++)
    {
       	if (valList[x].name.compare("dampValue")== 0)
       	{
       		dampValue = valList[x].value;
       		break;
       	}
    }
	return true;
}
		
	
void EntropyDrive::Cleanup ()
{
	MotorDriveLeft1->Disable();
	MotorDriveLeft2->Disable();
	MotorDriveRight1->Disable();
	MotorDriveRight2->Disable();
}
	
		
bool EntropyDrive::DriveRobot(float MoveValue, float RotateValue){

	float LeftMotors = 0; 
	float RightMotors = 0; 

	MoveValue = Limit(MoveValue);
	RotateValue = Limit(RotateValue);

	MoveValue = addDeadZone(MoveValue);
	//MoveValue = moveValueDampen(MoveValue);

	LeftMotors = left_scale(RotateValue, MoveValue, Rotate);  
	RightMotors = right_scale(RotateValue, MoveValue, Rotate);
	
	//DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "Drive L: %f", LeftMotors);
	//DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "Drive R: %f", RightMotors);
	//DriverStationLCD::GetInstance()->UpdateLCD();
	
	//Command motors
	wpiDrive->SetLeftRightMotorOutputs( LeftMotors, RightMotors );

	return true;
}
	
		
	
bool EntropyDrive::DriveRobotTrig(float MoveValue, float RotateValue){

	float LeftMotors = 0; 
	float RightMotors = 0; 
	float OutsideWheels = 0;
	float InsideWheels = 0;
	float Hypot = 0;
	float AbsMoveValue = 0;
	float AbsRotateValue = 0;



	//Normalize Joystick inputs
	if (MoveValue >= 0.0)
	{
		MoveValue=MoveValue/CompMoveValuePlus;
	}
	else
	{
		MoveValue=MoveValue/CompMoveValueMinus;
	}

	if (RotateValue>=0.0)
	{
		RotateValue=RotateValue/CompRotateValuePlus;
	}
	else
	{
		RotateValue=RotateValue/CompRotateValueMinus;
	}


	MoveValue=Limit(MoveValue);
	RotateValue=Limit(RotateValue);
	AbsMoveValue=absolutevalue(MoveValue);
	AbsRotateValue= absolutevalue(RotateValue);


	//Theta = atanf(AbsMoveValue/(AbsRotateValue+0.000001));
	//Theta = asinf(0.2);
	Hypot = sqrt(AbsMoveValue*AbsMoveValue+AbsRotateValue*AbsRotateValue);


	OutsideWheels = AbsMoveValue*(AbsMoveValue/Hypot);   
	InsideWheels = AbsMoveValue*( 1- (AbsRotateValue/Hypot));

	//Scale Motor inputs

	if (RotateValue<=0.0)
	{
		LeftMotors = InsideWheels * MoveValue/AbsMoveValue;
		RightMotors = OutsideWheels * MoveValue/AbsMoveValue;		
	}
	else
	{
		LeftMotors = OutsideWheels *  MoveValue/AbsMoveValue;
		RightMotors = InsideWheels * MoveValue/AbsMoveValue;
	}	



	//Command motors
	wpiDrive->SetLeftRightMotorOutputs( -1*LeftMotors,-1*RightMotors );

	return true;
}


/* search left drive table using binary search */
/* Input:   x_value  (rotate)*/
/*          y_value  (move -forward/backward)*/
/*          slow_mo  if true, scale output - not being done */
/* return: left scale_value */
float EntropyDrive::left_scale(float rotateValue, float moveValue, DriveMode mode)
{
	int x_index = 0;
	int y_index = 0;
	float temp_drive = 0;
	int x_idx = 0;
	float absRotate = rotateValue;

	if(mode == Radius)
	{
		absRotate = fabs(rotateValue);
	}	

	get_index(x_index, y_index, moveValue, absRotate, mode);

	if(mode == Rotate)
	{
		x_idx = x_index;
	}
	else
	{
		if(rotateValue < 0)
		{
			x_idx = 32-x_index;
		}
		else
		{
			x_idx = x_index;
		}

	}

	temp_drive = left_fast_njxy[y_index][x_idx];


	return temp_drive;
}


// Code is replicated from RobotDrive class
float EntropyDrive::Limit(float num)
{
	if (num > 1.0)
	{
		return 1.0;
	}
	if (num < -1.0)
	{
		return -1.0;
	}
	return num;
}



/* search right drive table using binary search if axis index tables */
/* Input:   x_value  (rotate)*/
/*          y_value  (move -forward/backward)*/
/*          slow_mo  if true, scale output - not being done */
/* return: left scale_value */
float EntropyDrive::right_scale(float rotateValue, float moveValue, DriveMode mode)
{
	int x_index = 0;
	int y_index = 0;
	float temp_drive = 0;
	int x_idx = 0;
	float absRotate = rotateValue;

	if(mode == Radius)
	{
		absRotate = fabs(rotateValue);
	}

	get_index(x_index, y_index, moveValue, absRotate, mode);

	temp_drive = left_fast_njxy[y_index][32-x_index];

	if(mode == Rotate)
	{
		x_idx = 32-x_index;
	}
	else
	{
		if(rotateValue < 0)
		{
			x_idx = x_index;
		}
		else
		{
			x_idx = 32-x_index;
		}
	}

	temp_drive = left_fast_njxy[y_index][x_idx];


	return temp_drive;
}


float EntropyDrive::absolutevalue(float x)
{
	if (x < 0.0) 
	{
		x = x*-1.0;
	}
	return x;
}

bool EntropyDrive::range(float x, float y, float z) 
{  
	return (((y <= x) && (x <= z)) || ((y >= x) && (x >= z)));
}




float EntropyDrive::drive_table_limit(float x, float max, float min)
{
	if(x > max)
	{
		return max;
	}
	else if(x < min)
	{
		return min;
	}
	else
	{
		return x;
	}
}


void EntropyDrive::get_index(int &x_index, int &y_index, float moveValue, float rotateValue, DriveMode mode)
{
	float rotate = 0;
	float move = 0;
	float minRotate = 0;
	float maxRotate = 0;
	const float *arrayPtr = 0;
	unsigned int arrayLength = 0;
	float diff1 = 0;
	float diff2 = 0;

	if(mode == Radius)
	{
		arrayPtr = left_lookup_radius;
		arrayLength = 18;//sizeof(left_lookup_radius)/sizeof(float);
		minRotate = arrayPtr[16];
		maxRotate = arrayPtr[15];
	}
	else /*Rotate*/
	{
		arrayPtr = left_lookupx;
		arrayLength = sizeof(left_lookupx)/sizeof(float);
		minRotate = arrayPtr[0];
		maxRotate = arrayPtr[arrayLength-1];
	}

	rotate = drive_table_limit(rotateValue, maxRotate, minRotate);

	for(unsigned int i = 0; i < arrayLength; i++) 
	{
		if(i+1 >= arrayLength || range(rotate, arrayPtr[i], arrayPtr[i+1]))
		{
			//Assume match found
			if((i + 1) >= arrayLength)
			{
				x_index = i;	
			}
			else
			{
				diff1 = fabs(rotate - arrayPtr[i]);
				diff2 = fabs(rotate - arrayPtr[i+1]);

				if(diff1 < diff2)
				{
					x_index = i;
				}
				else
				{
					x_index = i + 1;
				}
			}
			break;
		}
	}

	arrayLength = (sizeof(left_lookupy)/sizeof(float));
	move = drive_table_limit(moveValue, left_lookupy[32], left_lookupy[0]);

	for(unsigned int i = 0; i < arrayLength; i++) 
	{
		if(i+1 >= arrayLength || range(move, left_lookupy[i], left_lookupy[i+1]))
		{
			//Assume match found
			if((i + 1) >= arrayLength)
			{
				y_index = i;	
			}
			else
			{
				diff1 = fabs(move - left_lookupy[i]);
				diff2 = fabs(move - left_lookupy[i+1]);

				if(diff1 < diff2)
				{
					y_index = i;
				}
				else
				{
					y_index = i + 1;
				}
			}
			break;
		}
	}
}

double EntropyDrive::addDeadZone (double Value)
{
#ifdef DEADZONE

	if (Value<DEAD_ZONE_MAX){
		if (Value>-DEAD_ZONE_MAX){
			Value=0;
		}
	}

#endif	
	return Value;
}


float EntropyDrive::moveValueDampen (float moveValue)
{

	

	if ((moveValue - previousValue > -0.1 and moveValue - previousValue < 0.1) and moveValue == 0){
		previousValue = 0;
		moveValue = 0;
	}	 
	else if(moveValue > previousValue){

		moveValue = previousValue + dampValue;
	}	 

	else if (moveValue < previousValue){
		moveValue = previousValue - dampValue;
	}


	previousValue=moveValue;
	return moveValue;	


}

void EntropyDrive::DriveTrainTest( ) 
{ 

	CANJaguar * Motors[4] = {MotorDriveLeft1, MotorDriveLeft2, MotorDriveRight1, MotorDriveRight2 };
	char * Motors_Names[4] = {"MotorLeft1", "MotorLeft2", "MotorRight1", "MotorRight2" };
	int Motors_Values[4] = { IODefinitions::MOTOR_DRIVE_LEFT_1,
			IODefinitions::MOTOR_DRIVE_LEFT_2,
			IODefinitions::MOTOR_DRIVE_RIGHT_1,
			IODefinitions::MOTOR_DRIVE_RIGHT_2
	};

	UINT8 syncGroup = 0x80; 
	for ( int i = 0; i < 4; i++ )
	{
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1,"Motor: %s",
				 Motors_Names[i] );
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2,"Jaguar: %d", Motors_Values[i]);
		DriverStationLCD::GetInstance()->UpdateLCD();
		Motors[i]->Set( 0.6 ,syncGroup);
		CANJaguar::UpdateSyncGroup(syncGroup);
		Wait( 5 );
		Motors[i]->Set( 0.0 , syncGroup);
		CANJaguar::UpdateSyncGroup(syncGroup);
		Wait( 2 );
		DisplayEncodersTestDSLine5Line6();
	}
}

void EntropyDrive::InitEncoderTest()
{
	m_leftEncoderTest = new Encoder(1, 1, 1, 2, false, Encoder::k4X);
	m_rightEncoderTest = new Encoder(1, 3, 1, 4, true, Encoder::k4X);
		 
	m_leftEncoderTest->SetDistancePerPulse(PULSE_RATIO / 12.0);
	m_leftEncoderTest->SetPIDSourceParameter(Encoder::kRate);
	m_rightEncoderTest->SetDistancePerPulse(PULSE_RATIO / 12.0);
	m_rightEncoderTest->SetPIDSourceParameter(Encoder::kRate);
							 
	m_leftEncoderTest->Start();
	m_rightEncoderTest->Start();

}
	
	
void EntropyDrive::DisplayEncodersTestDSLine5Line6()
{
	  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "Left Dist: %f", m_leftEncoderTest->GetDistance());
	  DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "Right Dist: %f", m_rightEncoderTest->GetDistance());
	  DriverStationLCD::GetInstance()->UpdateLCD();
}
