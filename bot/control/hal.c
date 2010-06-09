/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * HAL source file
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h" 		// The RP6 Control Library. 
								// Always needs to be included!

#include "RP6I2CmasterTWI.h"	// I2C Master Library
#include "RP6I2CMasterLibExt.h"
#define I2C_RP6_BASE_ADR 10	
#define SPEED 80
#include "hal.h"

/*****************************************************************************/

direction compassDirection = NORTH;

uint8_t halX;
uint8_t halY;

direction hal_direction(void)
{
#if 0
    I2CTWI_transmitByte(0x42, 0x41);
	I2CTWI_transmitByte(0x42, 0x41);//2x voor actuele waarde
	uint8_t compass[2];
	I2CTWI_readBytes(0x43, compass, 2);
	uint16_t compas = (compass[0]<<8) + compass[1];
	if(compas > 3150 || compas < 450)
	{
		compassDirection = NORTH;
	}
	else if(compas > 450 && compas < 1350)
	{
		compassDirection = EAST;
	}
	else if(compas > 1350 && compas < 2250)
	{
		compassDirection = SOUTH;
	}
	else if(compas > 2250 && compas < 3150)
	{
		compassDirection = WEST;
	}
	setCursorPosLCD(1, 7);
	writeIntegerLCD(compassDirection, DEC);
#endif
    
    return compassDirection;
}

static void updateDirection(void)
{
    hal_direction();

    switch (compassDirection) {
        case NORTH:
            halY--;
            break;
        case WEST:
            halX--;
            break;
        case SOUTH:
            halY++;
            break;
        case EAST:
            halX++;
            break;
    }
}

void updateRichting(uint8_t dir)
{
    switch (compassDirection) {
        case NORTH:
            if (dir == 1) {
                compassDirection = WEST;
            }
            else {
                compassDirection = EAST;
            }
            break;
        case WEST:
            if (dir == 1) {
                compassDirection = SOUTH;
            }
            else {
                compassDirection = NORTH;
            }
            break;
        case SOUTH:
            if (dir == 1) {
                compassDirection = EAST;
            }
            else {
                compassDirection = WEST;
            }
            break;
        case EAST:
            if (dir == 1) {
                compassDirection = NORTH;
            }
            else {
                compassDirection = SOUTH;
            }
            break;
    }
}


uint8_t hal_getX(void)
{
    return halX;
}
uint8_t hal_getY(void)
{
    return halY;
}

void hal_turnLeft (void)
{
    rotate(50, LEFT, 95, BLOCKING);
    updateRichting(1);
}
void hal_turnRight (void)
{
    rotate(50, RIGHT, 95, BLOCKING);
    updateRichting(2);
}
void hal_moveForward(void)
{
    move(SPEED, FWD, DIST_MM(420), BLOCKING);
    updateDirection();
}

uint8_t wallRight = 0;
uint8_t wallLeft  = 0;
uint8_t wallFront = 0;

uint8_t hal_hasWallRight(void)
{
    return wallRight;
}
uint8_t hal_hasWallLeft(void)
{
    return wallLeft;
}
uint8_t hal_hasWallFront(void)
{
    return wallFront;
}

void hal_scan(void)
{
    // left, right
    uint16_t rechts = readADC(ADC_3);
    wallRight = (rechts > 100);

    uint16_t links = readADC(ADC_2);
    wallLeft = (links > 100);

    //wallFront = ((links < 898) || (rechts < 917));

    // put the stuff on the LCD
    clearLCD();

#if 0
    if (obstacle_left) {
        setCursorPosLCD(1, 0);
        writeCharLCD('M');
    }
    if (obstacle_right) {
        setCursorPosLCD(1, 15);
        writeCharLCD('M');
    }
    if (wallLeft) {
        setCursorPosLCD(1, 3);
        writeCharLCD('L');
    }
    if (wallRight) {
        setCursorPosLCD(1, 12);
        writeCharLCD('R');
    }
#endif
    wallFront = (obstacle_left && obstacle_right);
    
#if 0
    if (wallFront) {
        setCursorPosLCD(1, 12);
        writeCharLCD('F');
    }
#endif
}

void hal_init(uint8_t x, uint8_t y)
{
    halX = x;
    halY = y;

    I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation

	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_LOW);
	sleep(50);
	hal_direction();
}
