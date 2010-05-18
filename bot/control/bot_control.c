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

// stuff for the Two-Wire-Interface (I2C)
#include "RP6I2CmasterTWI.h"
#include "RP6Control_I2CMasterLib.h"

// other files for this project
#include "hal.h"
#include "ir.h"
#include "pathfinder.h"

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

/**
 * The I2C_requestedDataReady Event Handler
 */
void I2C_requestedDataReady(uint8_t dataRequestID)
{
	checkRP6Status(dataRequestID);
}


/**
 * Initialize the two-wire-interface (Also called I2C)
 */
void initTWI(void)
{
    I2CTWI_initMaster(100);

    // two-wire-interface event handlers
    I2CTWI_setRequestedDataReadyHandler(I2C_requestedDataReady);
	I2CTWI_setTransmissionErrorHandler(I2C_transmissionError);
}

/**
 * Give the user a nice and warm welcome
 */
void welcome(void)
{
	// Play two sounds:
	sound(180,80,25);
	sound(220,80,25);

	setLEDs(0b1111); // Turn all LEDs on!

	showScreenLCD("################", "################");
	mSleep(500);
	showScreenLCD("RoboHeroes", "Rescue Robot");
	mSleep(1000);
	// ---------------------------------------
	setLEDs(0b0000); // All LEDs off!
}

int main(void)
{
	initRP6Control();
	initLCD();
    initTWI();

    welcome();

	while(true)
	{
        // for now, we do nothing
	}
	return 0;
}

