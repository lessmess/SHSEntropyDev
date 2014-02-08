#ifndef IO_DEFINITIONS_H_
#define IO_DEFINITIONS_H_


class IODefinitions {
	
public:
	
	enum {
		COMPRESSOR_RELAY_CHANNEL = 1,
		PRESSURE_SWITCH_CHANNEL = 8,
		PRESSURE_SWITCH_MODULE = 1,
		COMPRESSOR_RELAY_MODULE =1,
	} Compressor;

	enum {
		UPPER_SOLONOID_CHANNEL = 1,
		LOWER_SOLONOID_CHANNEL = 2,
		KICKER_SOLONOID = 3
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
		KICKER_PULL_CAN_JAG,
		MOTOR_ACQUISITION_BELT = 6,
		MOTOR_ACQUISITION_EXTENDER = 7,
		UNUSED_8,
		UNUSED_9,
		UNUSED_10
	} CanBus;

    enum {
    	USB_PORT_1 = 1,
    	USB_PORT_2,
    	USB_PORT_3
    	
    	
    } Physical_USB_Port;
    
    /**
     *  TODO: Map the correct buttons from the controller to this. 
     */
    enum { 
    	GAME_BUTTON_NOT_USED         = 0,
    	UPPER_SOLONOID_BUTTON        = 1,
    	LOWER_SOLONOID_BUTTON        = 2,
    	KICKER_BUTTON_1              = 2,
    	KICKER_BUTTON_2              = 2,
    	GAME_BUTTON_ARM_UP           = 3,
    	GAME_BUTTON_ARM_DOWN         = 4,
    	GAME_BUTTON_ARM_IN           = 5,
    	GAME_BUTTON_ARM_OUT          = 6,
    	
    } Game_Stick_IO;
    
    /**
     * TODO: Map the correct buttons from the drive stick to this. 
     */
    enum {
    	DRIVE_BUTTON_A, 
    	DRIVE_BUTTON_B
    } Drive_Stick_IO;
    
    enum {
    	PULL_SWITCH_DIGITAL_CHANNEL,
    	PULL_SWITCH_DIGITAL_MODULE
    };
};

#endif
