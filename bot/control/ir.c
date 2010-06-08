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

#if 0
void ir_sendBaseStation()
{
	I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 27); 
	uint8_t divice_bit = I2CTWI_readByte(I2C_RP6_BASE_ADR);
	
	SPI_EEPROM_readBytes(lees, transmit_buffer, 8);
	mSleep(10);
	
    if (divice_bit == 8 || divice_bit == 9) {
        IRCOMM_sendRC5(9,transmit_buffer[0]);
        mSleep(10);
        IRCOMM_sendRC5(9,transmit_buffer[1]);
        mSleep(10);
        IRCOMM_sendRC5(9,transmit_buffer[2]);
        mSleep(10);
        IRCOMM_sendRC5(9,transmit_buffer[3]);
    } else if (divice_bit == 16 || divice_bit == 17) {
        IRCOMM_sendRC5(17,transmit_buffer[0]);
        mSleep(10);
        IRCOMM_sendRC5(17,transmit_buffer[1]);
        mSleep(10);
        IRCOMM_sendRC5(17,transmit_buffer[2]);
        mSleep(10);
        IRCOMM_sendRC5(17,transmit_buffer[3]);
    }
	
	lees = (lees + 8);
}

void ir_receiveBaseStation()
{
	I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 28);
	uint8_t tijdelijk = I2CTWI_readByte(I2C_RP6_BASE_ADR);

    // Oops! i did it again (van 44 naar 9)
    if (tijdelijk >= 48 && tijdelijk <= 56) {
        startY = tijdelijk - 48;
    } else if (tijdelijk >= 32 && tijdelijk <= 38) {
        startX = tijdelijk - 32;
    } else if (tijdelijk >= 16 && tijdelijk <= 24) {
        personY = tijdelijk - 16;
    } else if (tijdelijk <= 6) {
        personX = tijdelijk;
    }
}


void ir_sendSituation(direction dir, uint8_t x, uint8_t y)
{
	// hal_hasWallLeft() hal_hasWallRight() hal_hasWallFront()
	receive_buffer[0] = x;
	receive_buffer[1] = y+16;

	// direction
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

	// wall
    receive_buffer[3] = 40 + (hal_hasWallLeft() << 2) + (hal_hasWallFront() << 1) + hal_hasWallRight();
#if 0
    L F R
    0 0 0   0
    0 0 1   1
    0 1 0   2
    0 1 1   3
    1 0 0   4
    1 0 1   5
    1 1 0   6
    1 1 1   7
#endif

	SPI_EEPROM_writeBytes(0, receive_buffer, schrijf);

	schrijf = (schrijf + 8);
}
#endif
