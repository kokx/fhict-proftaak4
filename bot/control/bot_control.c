/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Bot control header file
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


// Main function - The program starts here:

int main(void)
{
	initRP6Control(); // Always call this first! The Processor will not work
					  // correctly otherwise. 

	initLCD(); // Initialize the LC-Display (LCD)
			   // Always call this before using the LCD!
			   
	writeString_P("\n\nFHICT Proftaak 4 Program!\n"); 

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
	showScreenLCD("I2C-Master", "Rescue Robot");
	mSleep(1000);
	// ---------------------------------------
	setLEDs(0b0000); // All LEDs off!
	
	uint8_t counter = 1;

	while(true) 
	{
        // for now, we do nothing
	}
	return 0;
}

