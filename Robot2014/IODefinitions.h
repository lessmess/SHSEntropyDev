#ifndef IO_DEFINITIONS_H_
#define IO_DEFINITIONS_H_


class IODefinitions {
	
public:
	
	enum {
		COMPRESSOR_RELAY_CHANNEL = 1,   //Resolve with Tim
		PRESSURE_SWITCH_CHANNEL = 8,
		PRESSURE_SWITCH_MODULE = 1,
		COMPRESSOR_RELAY_MODULE =1,
	} Compressor;

	
	enum {
		//UPPER_SOLONOID_CHANNEL = 1,  // Remove
		//LOWER_SOLONOID_CHANNEL = 2,
		ARM_RAIL = 1,
		ARM_ROTATION = 3,
		KICKER_SHIFTER = 5, 
		KICKER_TRIGGER = 7 //Renamed
	}Solonoid;
	
	enum {
		AUTONOMOUS_SELECTOR_SWITCH = 6
	}Selector_Switch;

	
	enum  {
		UNUSED_0 =0,
		UNUSED_1,
		MOTOR_DRIVE_LEFT_1,
		MOTOR_DRIVE_LEFT_2,
		MOTOR_DRIVE_RIGHT_1,
		MOTOR_DRIVE_RIGHT_2,
		UNUSED_5,
		UNUSED_6
	} CanBus;

	enum  {
		PWM_UNUSED_0 =0,
		MOTOR_ACQUISITION_EXTENDER = 1,
		PWM_UNUSED_2,
		MOTOR_ACQUISITION_BELT = 3,
		PWM_UNUSED_4,
		KICKER_PULL,
		PWM_UNUSED_6,
		UNUSED_7
	} PWM_MOTORS;

    enum {
    	USB_PORT_1 = 1,
    	USB_PORT_2
    } Physical_USB_Port;
    
    
	enum { 
    	GAME_BUTTON_NOT_USED         = 0,
    	KICKER_TRIGGERKICK           = 2,
    	KICKER_PREPAREKICK           = 3,
		GAME_BUTTON_ARM_ROLL_IN      = 3,
		GAME_BUTTON_ARM_ROLL_OUT     = 2,
		GAME_BUTTON_ARM_UP           = 5,
    	GAME_BUTTON_ARM_DOWN         = 7,
    	GAME_BUTTON_ARM_IN           = 6,
    	GAME_BUTTON_ARM_OUT          = 8  
    	
    } Game_Stick_IO;
    
    enum {
    	DRIVE_BUTTON_A, // No buttons used on the drive stick.
    	DRIVE_BUTTON_B
    } Drive_Stick_IO;
    
    enum {
    	PULL_SWITCH_DIGITAL_CHANNEL,   // What is this ??
    	PULL_SWITCH_DIGITAL_MODULE
    };
};

#endif
