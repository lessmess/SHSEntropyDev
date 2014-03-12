#include "WPILib.h"
#include "IODefinitions.h"
#include "EntropyDrive.h"
#include "EntropyJoystick.h"
#include "ExampleSHS.h"
#include "EntropyInfraredSensor.h"
#include "AcquisitionArms.h"
#include "GenericHID.h"
#include "Math.h"
#include "Kicker.h"
#include "EntropyCompressor.h"
#include "EntropyCamera.h"
#include "EntropyRangeFinder.h"
#include "Autonomy.h"
#include "Gyro.h"


class EntropyRobot2014 : public IterativeRobot
{
	// Declare variable for the robot system
		
	// Declare a variable to use to access the driver station object
	DriverStation *EntropyDriverStation;			// driver station object
	UINT32 m_priorPacketNumber;					// keep track of the most recent packet number from the DS
	UINT8 m_dsPacketsReceivedInCurrentSecond;	// keep track of the ds packets received in the current second
	
	// Declare variables for the two joysticks being used
	EntropyJoystick *DriveStick;			// EntropyJoystick used for robot driving
	EntropyJoystick *GameStick;			// EntropyJoystick for all other functions		
	float m_turnSpeed;
	
	//Output to Driver Station;
	DriverStationLCD *ds; 
	
	// Declare SHS Subsystems here
	EntropyCompressor MyCompressor;
	EntropyDrive MyRobot;		// The Robot Drive instance
	AcquisitionArms Arm;	
    Kicker MyKicker; 			//The Shooter instance
    EntropyCamera MyCameraControl;
    EntropyRangeFinder MyRangeFinder;
    Autonomy* MyAutoRobot;
    
	// Local variables to count the number of periodic loops performed
	UINT32 m_autoPeriodicLoops;
	UINT32 m_disabledPeriodicLoops;
	UINT32 m_telePeriodicLoops;
	
	bool autoKicked;
	bool didOnce;
	//Creating a test "InfraredSensor", by way of using an Autonomous Selector Switch
	EntropyInfraredSensor InfraredSensor;
	
	Gyro* m_Gyro;

public:
/**
 * Constructor for this "EntropyRobotDrive2014" Class.
 */
	EntropyRobot2014(void)	
	{
		printf(" Constructor Started\n");

		// Establish Hardware IO Controllers
		DriveStick = new EntropyJoystick(IODefinitions::USB_PORT_1);
		GameStick = new EntropyJoystick(IODefinitions::USB_PORT_2);		
	    
		AxisCamera* axisCam = &AxisCamera::GetInstance();
		
		 DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "Don't Start Yet");
				 			 DriverStationLCD::GetInstance()->UpdateLCD();
		 m_Gyro = new Gyro(1);    //This takes like 5 or 6 seconds... sorry
		 DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "Ready");
		 			 DriverStationLCD::GetInstance()->UpdateLCD();
		 m_Gyro->SetSensitivity(.00577);
		
		MyCompressor.Initialize();
		MyRangeFinder.Initialize();	
		InfraredSensor.Initialize();
			
		// Acquire the Driver Station object
		EntropyDriverStation = DriverStation::GetInstance();
		m_priorPacketNumber = 0;
		m_dsPacketsReceivedInCurrentSecond = 0;

		// Initialize counters to record the number of loops completed in autonomous and teleop modes
		m_autoPeriodicLoops = 0;
		m_disabledPeriodicLoops = 0;
		m_telePeriodicLoops = 0;
		
		autoKicked = false;
		didOnce = false;
		
		ds = DriverStationLCD::GetInstance();
		
		m_turnSpeed=1.0;
		
		
		printf("EntropyBot14 Constructor Completed\n");

		MyCameraControl.SetCameraPositionAuto();
	}
	
	
	/********************************** Init Routines *************************************/

	void RobotInit(void) 
	{
		// Actions which would be performed once (and only once) upon initialization of the
		// robot would be put here.
		
		
		// Initialize SHS Subsystems here
		MyRobot.Initialize();
		MyCompressor.Initialize();
		Arm.Initialize();
		InfraredSensor.Initialize();
        MyKicker.Initialize();
        MyCameraControl.Initialize();
        MyRangeFinder.Initialize();
        
		printf("RobotInit() completed.\n");
	}
	
	void DisabledInit(void) 
	{
		m_disabledPeriodicLoops = 0;			// Reset the loop counter for disabled mode
	}

	void AutonomousInit(void) 
	{
		m_autoPeriodicLoops = 0;				// Reset the loop counter for autonomous mode
 -		MyCameraControl.SetCameraPositionAuto();
		Arm.SetAutoInitialState();
		MyAutoRobot = new Autonomy(MyRobot, m_Gyro);
	}

	void TeleopInit(void) 
	{
		m_telePeriodicLoops = 0;				// Reset the loop counter for teleop mode
		m_dsPacketsReceivedInCurrentSecond = 0;	// Reset the number of dsPackets in current second

		MyCameraControl.SetCameraPositionTelop();
	}

	/********************************** Periodic Routines *************************************/
	
	void DisabledPeriodic(void)  
	{
		
		//Disable Drive
		MyRobot.Cleanup();
		Arm.Cleanup();
		MyKicker.Cleanup();
		InfraredSensor.Cleanup();
		
		DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Kicker: %s", "Disabled");
		DriverStationLCD::GetInstance()->UpdateLCD();
	}

	void AutonomousPeriodic(void) 
	{
		m_autoPeriodicLoops++;
		
		static double autoEpoch = GetClock();
		
		if( GetClock() - autoEpoch > 0.5)
		{
			if (didOnce == false)
			{
				HSLImage *image = AxisCamera::GetInstance().GetImage();
				image->Write("potato3.bmp");
				didOnce = true;
			}
			
			MyAutoRobot->Update(GetClock() - autoEpoch);
	
			if (MyKicker.getKickerState() != Kicker::readytoshoot && autoKicked == false)
			{
				MyKicker.Kick(true, false);
			}
			else if(autoKicked == false && GetClock() - autoEpoch > 7.6)
			{
				MyKicker.Kick(false, true);
	
				autoKicked = true;
			}
		}
	}

	void TeleopPeriodic(void) 
	{
		// increment the number of teleop periodic loops completed
		m_telePeriodicLoops++;
		
		MyCameraControl.SetCameraPositionTelop();
		MyRangeFinder.UpdateRangeLine4DS();
		InfraredSensor.UpdateRangeLine5DS();
		
		//Feed joystick inputs to each subsystem here
        MyKicker.Kick(GameStick->GetRawButton(IODefinitions::KICKER_PREPAREKICK), 
        GameStick->GetRawButton(IODefinitions::KICKER_TRIGGERKICK));
        
        MyRobot.DriveRobot(DriveStick->GetY(),DriveStick->GetRawAxis(4));
		
		Arm.UpperVerticalPos(GameStick);
		Arm.LowerVerticalPos(GameStick);
		Arm.Extend(GameStick);
		Arm.BeltEnable(GameStick);

		
	} // TeleopPeriodic(void) 
			
};

START_ROBOT_CLASS(EntropyRobot2014);
