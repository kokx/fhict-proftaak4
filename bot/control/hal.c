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

direction compassDirection;
//noord = 1
// zuid = 2
// oost = 8
// west = 4
// dir 1 = links
// dir 2 = rechts
void beginRichting(void)
{
	I2CTWI_transmitByte(0x42, 0x41);
	I2CTWI_transmitByte(0x42, 0x41);//2x voor actuele waarde
	uint8_t compass[2];
	I2CTWI_readBytes(0x43, compass, 2);
	uint16_t richting = (compass[0]<<8) + compass[1];
	if(richting == 710)
	{
		compassDirection = NORTH;
	}
	else if (richting == 510)
	{
		compassDirection = SOUTH;
	}
}

void set_direction(uint8_t dir)
{
	if(compassDirection == NORTH) {
		if(dir == 1)
		{
			compassDirection = WEST;
		}
		if(dir == 2)
		{
			compassDirection = EAST;
		}
	} else if(compassDirection == SOUTH) {
		if(dir == 1)
		{
			compassDirection = EAST;
		}
		if(dir == 2)
		{
			compassDirection = WEST;
		}
	} else if(compassDirection == WEST) {
		if(dir == 1)
		{
			compassDirection = NORTH;
		}
		if(dir == 2)
		{
			compassDirection = SOUTH;
		}
	} else if(compassDirection == EAST) {
		if(dir == 1)
		{
			compassDirection = SOUTH;
		}
		if(dir == 2)
		{
			compassDirection = NORTH;
		}
	}	
}



void readCompass(void)
{
	I2CTWI_transmitByte(0x42, 0x41);
	I2CTWI_transmitByte(0x42, 0x41);//2x voor actuele waarde
	uint8_t compass[2];
	I2CTWI_readBytes(0x43, compass, 2);
	writeInteger(compass[0], DEC);
	writeInteger(compass[1], DEC);
	setCursorPosLCD(1, 3);
	writeIntegerLengthLCD((compass[0]<<8) + compass[1], DEC, 4);
}


void hal_turnLeft (void)
{
    rotate(50, LEFT, 90, BLOCKING);
	set_direction(1);
}
void hal_turnRight (void)
{
    rotate(50, RIGHT, 90, BLOCKING);
	set_direction(2);
}

void hal_check(void)
{
	uint16_t adc3 = readADC(ADC_3);
	uint16_t adc2 = readADC(ADC_2); 
	uint16_t verschil;
	if(adc3 > adc2)
	{
		verschil = adc3 - adc2;
		hal_turnLeft();
		move(30, FWD, DIST_MM(20), BLOCKING);
		hal_turnRight();
		

	}
	else if(adc2 > adc3)
	{
		verschil = adc2 - adc3;
		hal_turnRight();
		move(30, FWD, DIST_MM(20), BLOCKING);
		hal_turnLeft();
	}
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

static void writeBooleanToLcd(uint8_t bool)
{
    if (bool) {
        writeCharLCD('Y');
    } else {
        writeCharLCD('N');
    }
}

void hal_scan(void)
{
    // left, right
    uint16_t rechts = readADC(ADC_3);
    wallRight = (rechts > 100);

    uint16_t links = readADC(ADC_2);
    wallLeft = (links > 100);

    uint8_t lightSens[4];

	I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 13); // Start with register 13 (LSL_L)...
	I2CTWI_readBytes(I2C_RP6_BASE_ADR, lightSens, 4); // and read all 4 registers up to

	links = lightSens[0] + (lightSens[1]<<8) ;
	rechts = lightSens[2] + (lightSens[3]<<8);

    wallFront = ((links < 898) && (rechts < 917));

    // put the stuff on the LCD
    clearLCD();

    setCursorPosLCD(1, 14);
    writeBooleanToLcd(wallRight);
    setCursorPosLCD(0, 7);
    writeBooleanToLcd(wallFront);
    setCursorPosLCD(1, 1);
    writeBooleanToLcd(wallLeft);
}

void hal_moveForward(void)
{
    move(SPEED, FWD, DIST_MM(420), BLOCKING);
}

void hal_init(void)
{
    I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation

	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_LOW);
	sleep(50);
}
