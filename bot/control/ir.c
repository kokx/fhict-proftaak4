

#include "RP6ControlLib.h"

#include "RP6I2CmasterTWI.h"

#include "hal.h"

#define RP6BASE_I2C_SLAVE_ADR 10

uint8_t transmit_buffer[8];
uint8_t receive_buffer[8];
uint8_t lees = 64;
uint8_t schrijf = 64;
uint8_t personX;
uint8_t personY;
uint8_t startX;
uint8_t startY;

void ir_sendBaseStation()
{
	SPI_EEPROM_readBytes(lees, transmit_buffer, 8);
	I2CTWI_transmitBytes(RP6BASE_I2C_SLAVE_ADR, transmit_buffer, 8);

	lees = (lees + 8);
}

void ir_receiveBaseStation()
{
	I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 28);
	uint8_t tijdelijk = I2CTWI_readByte(I2C_RP6_BASE_ADR);

	switch(tijdelijk)
	{
		//persoonX
		case 0:	personX=0; break;
		case 1:	personX=1; break;
		case 2: personX=2; break;
		case 3: personX=3; break;
		case 4:	personX=4; break;
		case 5: personX=5; break;
		case 6: personX=6; break;

		//persoonY
		case 16: personY=0; break;
		case 17: personY=1; break;
		case 18: personY=2; break;
		case 19: personY=3; break;
		case 20: personY=4; break;
		case 21: personY=5; break;
		case 22: personY=6; break;
		case 23: personY=7; break;
		case 24: personY=8; break;

		//startX
		case 32: startX=0; break;
		case 33: startX=1; break;
		case 34: startX=2; break;
		case 35: startX=3; break;
		case 36: startX=4; break;
		case 37: startX=5; break;
		case 38: startX=6; break;

		//startY
		case 48: startY=0; break;
		case 49: startY=1; break;
		case 50: startY=2; break;
		case 51: startY=3; break;
		case 52: startY=4; break;
		case 53: startY=5; break;
		case 54: startY=6; break;
		case 55: startY=7; break;
		case 56: startY=8; break;

		default: /*niets*/ break;
	}
}


void ir_sendSituation(direction dir, uint8_t x, uint8_t y)
{
	// hal_hasWallLeft() hal_hasWallRight() hal_hasWallFront()
	receive_buffer[0] = x;
	receive_buffer[1] = y+16;

	// direction
    // alex kan blijkbaar niet coden !!!!
	if (dir == NORTH){
        receive_buffer[2] = 32;
    } else if (dir == EAST) {
        receive_buffer[2] = 33;
    } else if (dir == SOUTH) {
        receive_buffer[2] = 34;
    } else if (dir == WEST) {
        receive_buffer[2] = 35;
    }

    // pro-tip: het maakt geen zak uit hoe veel lijnen een programma is

	// wall (van 17 lijnen naar 1 lijn!)
    receive_buffer[3] = 40 + (hal_hasWallLeft() << 2) + (hal_hasWallFront() << 1) + hal_hasWallRight();
#if 0
 	if (hal_hasWallLeft() == false && hal_hasWallRight() == false && hal_hasWallFront() == false) {
        receive_buffer[3] = 40;
    } else if (hal_hasWallLeft() == false && hal_hasWallRight() == true && hal_hasWallFront() == false) {
        receive_buffer[3] = 41;
    } else if (hal_hasWallLeft() == false && hal_hasWallRight() == false && hal_hasWallFront() == true) {
        receive_buffer[3] = 42;
    } else if (hal_hasWallLeft() == false && hal_hasWallRight() == true && hal_hasWallFront() == true) {
        receive_buffer[3] = 43;
    } else if (hal_hasWallLeft() == true && hal_hasWallRight() == false && hal_hasWallFront() == false) {
        receive_buffer[3] = 44;
    } else if (hal_hasWallLeft() == true && hal_hasWallRight() == true && hal_hasWallFront() == false) {
        receive_buffer[3] = 45;
    } else if (hal_hasWallLeft() == true && hal_hasWallRight() == false && hal_hasWallFront() == true) {
        receive_buffer[3] = 46;
    } else if (hal_hasWallLeft() == true && hal_hasWallRight() == true && hal_hasWallFront() == true) {
        receive_buffer[3] = 47;
    }

    // Laat ik maar es proberen of ik het netjes neer kan zetten
    
    // oke alex: je kan echt niet coden, zie dit:
    L F R
    0 0 0   0
    0 0 1   1
    0 1 0   2
    0 1 1   3
    1 0 0   4
    1 0 1   5
    1 1 0   6
    1 1 1   7

    // oke, dan tellen we er wel netjes 40 bij op :P
#endif

	SPI_EEPROM_writeBytes(0, receive_buffer, schrijf);

	schrijf = (schrijf + 8);
}
