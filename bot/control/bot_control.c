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
#include "RP6I2CMasterLibExt.h"

// other files for this project
#include "hal.h"
#include "ir.h"
#include "pathfinder.h"

// states
#define STATE_SEND      0
#define STATE_RECEIVE   1
#define STATE_MOVE      2
#define STATE_FINDPATH  3

uint8_t state;

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

/*
 * Initialize the ACS
 */
void initACS(void)
{
    //ACS_setStateChangedHandler(acsStateChanged);

	// ---------------------------------------
	// Setup ACS power:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_LOW); // ACS_POWER_MED
	// Enable Watchdog for Interrupt requests:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT, true);
	// Enable timed watchdog requests:
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_WDT_RQ, true);
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

direction currentDirection;

static void turnTo(direction dir)
{
    if (currentDirection == dir) {
        return;
    }
    switch (dir) {
        case NORTH:
            switch (currentDirection) {
                case SOUTH:
                    hal_turnLeft();
                    hal_turnLeft();
                    break;
                case WEST:
                    hal_turnLeft();
                    break;
                case EAST:
                    hal_turnRight();
                    break;
            }
            break;
        case WEST:
            switch (currentDirection) {
                case EAST:
                    hal_turnLeft();
                    hal_turnLeft();
                    break;
                case SOUTH:
                    hal_turnLeft();
                    break;
                case NORTH:
                    hal_turnRight();
                    break;
            }
            break;
        case SOUTH:
            switch (currentDirection) {
                case NORTH:
                    hal_turnLeft();
                    hal_turnLeft();
                    break;
                case EAST:
                    hal_turnLeft();
                    break;
                case WEST:
                    hal_turnRight();
                    break;
            }
            break;
        case EAST:
            switch (currentDirection) {
                case WEST:
                    hal_turnLeft();
                    hal_turnLeft();
                    break;
                case NORTH:
                    hal_turnLeft();
                    break;
                case SOUTH:
                    hal_turnRight();
                    break;
            }
            break;
    }
}

// temp function
static void writeDir(direction dir)
{
    setCursorPosLCD(0, 5);

    switch (dir) {
        case NORTH:
            writeCharLCD('N');
            break;
        case WEST:
            writeCharLCD('W');
            break;
        case SOUTH:
            writeCharLCD('S');
            break;
        case EAST:
            writeCharLCD('E');
            break;
    }
}

int main(void)
{
	initRP6Control();
	initLCD();
    initTWI();
    initACS();

    welcome();

    currentDirection = NORTH;
	
    // initialization of variables, will later happen with IR
    direction dir = NORTH;
    uint8_t x = 6;
    uint8_t y = 8;

    uint8_t targetX = 2;
    uint8_t targetY = 4;

	// initialize components
	hal_init(x, y);
	pathfinder_init(x, y, currentDirection); // X, Y, Direction
	// ir_init();
    
    pathfinder_setTarget(targetX, targetY);


    state = STATE_RECEIVE;

	while (true) {
        switch (state) {
            case STATE_RECEIVE:
                // receive stuff
                setCursorPosLCD(0, 0);
                writeCharLCD('R');

                state = STATE_SEND;
                break;
            case STATE_SEND:
                // send stuff
                setCursorPosLCD(0, 0);
                writeCharLCD('S');

                hal_scan();
                state = STATE_FINDPATH;
                break;
            case STATE_FINDPATH:
                setCursorPosLCD(0, 0);
                writeCharLCD('F');

                dir = pathfinder_NextStep(NORTH, x, y);

                writeDir(dir);
                mSleep(500);

                state = STATE_MOVE;
                break;
            case STATE_MOVE:
                setCursorPosLCD(0, 0);
                writeCharLCD('M');

                turnTo(dir);
                hal_moveForward();

                state = STATE_RECEIVE;
                break;
        }
	}

	return 0;
}

