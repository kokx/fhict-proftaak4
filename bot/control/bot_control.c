/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * Now we will use the I2C Bus Interface to send commands to the Controller
 * on the Mainboard and read sensor values. 
 * This program does not do anything directly, it checks for pressed buttons
 * and then runs a small I2C Bus example depending on which button has
 * been pressed. 
 *
 * You need to program the Controller on the Mainboard with the I2C Bus Slave
 * Example program from the RP6Base examples! Otherwise it will not work!
 *
 * ############################################################################
 * The Robot does NOT move in this example! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h" 		// The RP6 Control Library. 
								// Always needs to be included!

#include "RP6I2CmasterTWI.h"

/*****************************************************************************/

#define I2C_RP6_COMPASS 0x42    // compass address

/*****************************************************************************/
// I2C Error handler

/**
 * This function gets called automatically if there was an I2C Error like
 * the slave sent a "not acknowledge" (NACK, error codes e.g. 0x20 or 0x30).
 * The most common mistakes are: 
 *   - using the wrong address for the slave
 *   - slave not active or not connected to the I2C-Bus
 *   - too fast requests for a slower slave
 * Be sure to check this if you get I2C errors!
 */
void I2C_transmissionError(uint8_t errorState)
{
	writeString_P("\nI2C ERROR - TWI STATE: 0x");
	writeInteger(errorState, HEX);
	writeChar('\n');
}


/*****************************************************************************/
// Rotate function

uint8_t transmit_buffer[10]; // temporary transmit buffer
							 // A global variable saves space on the heap... 

/*****************************************************************************/
// Read all registers function

/** 
 * Here we demonstrate how to read a few specific registers. 
 * It is just the same as above, but we only read 4 registers and
 * start with register Number 13...
 * We also show how to combine values from high and low registers 
 * back together to a 16 Bit value... 
 */
void readCompass(void)
{
    uint8_t compass[2];

	I2CTWI_transmitByte(I2C_RP6_COMPASS, 0x41);
	I2CTWI_readBytes(I2C_RP6_COMPASS, compass, 2);

	writeString_P(":\n");
	writeString_P(" | low:"); writeInteger(compass[0], DEC); 
	writeString_P(" | high:"); writeInteger(compass[1], DEC); 

	writeString_P(" | compass value:"); writeInteger(compass[0] + (compass[1]<<8), DEC); 
	writeChar('\n');
	
	setCursorPosLCD(1, 3);
	writeIntegerLengthLCD(compass[1] + (compass[0]<<8), DEC, 4);
}

/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!
			   
	writeString_P("\n\nRP6 CONTROL M32 I2C Master Compass Program!\n"); 

	// IMPORTANT:
	I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation
							// with 100kHz SCL Frequency
							
	// Register the event handlers:
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);

	// Play two sounds:
	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111); // Turn all LEDs on!

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("I2C-Master", "Compass read program");
	mSleep(1000);
	// ---------------------------------------
	setLEDs(0b0000); // All LEDs off!
	
	uint8_t counter = 1;

	while(true) 
	{
		// We check the buttons - just like the buttons example - but here we 
		// generate several I2C Bus requests and commands when a key 
		// is pressed AND released again...
		uint8_t key = checkReleasedKeyEvent(); 
		
		if(key)
		{
			switch(key)
			{
				case 1:
					setLEDs(0b0100); 
					showScreenLCD("COMPASS:", "C:");
					readCompass();
				break;
			}
		}
	}
	return 0;
}

