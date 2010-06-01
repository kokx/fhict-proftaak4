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

uint8_t hal_hasWallRight(void)
{
	uint16_t rechts = readADC(ADC_2); 
	if(rechts>150)
	{
		writeString_P("\nmuur rechts");
		return 1;
	}
	else
	{
		return 0;
	}
}


uint8_t hal_hasWallLeft(void)
{
	uint16_t links = readADC(ADC_3);
	if(links>150)
	{
		writeString_P("\nmuur links");
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t hal_hasWallFront(void)
{
	uint8_t lightSens[4];

	I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 13); // Start with register 13 (LSL_L)...
	I2CTWI_readBytes(I2C_RP6_BASE_ADR, lightSens, 4); // and read all 4 registers up to
	
	uint16_t links; 
	links = lightSens[0] + (lightSens[1]<<8) ;
	uint16_t rechts; 
	rechts = lightSens[2] + (lightSens[3]<<8);

    if (links < 905 && rechts < 877)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void hal_moveForward(void)
{
    move(80, FWD, DIST_MM(210), BLOCKING);
	hal_check();
	move(80, FWD, DIST_MM(210), BLOCKING);
	hal_check();
}

void hal_init(void)
{
	initLCD();
		I2CTWI_initMaster(100); // Initialize the TWI Module for Master operation

	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_SET_ACS_POWER, ACS_PWR_LOW);
	sleep(50);
}
