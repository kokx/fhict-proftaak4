#include "RP6ControlLib.h"
#include "RP6I2CmasterTWI.h"
#include "hal.h"
#include "ir.h"

#define RP6BASE_I2C_SLAVE_ADR 10

// local vars
uint8_t transmit_buffer[8];
uint8_t receive_buffer[8];
uint8_t lees = 64;
uint8_t schrijf = 64;

// 'public' vars
uint8_t personX = 0xFF;
uint8_t personY = 0xFF;
uint8_t startX = 0xFF;
uint8_t startY = 0xFF;

// getters
uint8_t ir_getStartX(void)
{
    return startX;
}
uint8_t ir_getStartY(void)
{
    return startY;
}
uint8_t ir_getTargetX(void)
{
    return personX;
}
uint8_t ir_getTargetY(void)
{
    return personY;
}

// functions
void ir_sendBaseStation(void)
{
    if(lees<schrijf){
        I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 27); 
        uint8_t divice_bit = 8; //I2CTWI_readByte(I2C_RP6_BASE_ADR);

        transmit_buffer[0]= SPI_EEPROM_readByte(lees);
        transmit_buffer[1]= SPI_EEPROM_readByte(lees+1);
        transmit_buffer[2]= SPI_EEPROM_readByte(lees+2);
        transmit_buffer[3]= SPI_EEPROM_readByte(lees+3);
        mSleep(10);

        if (divice_bit == 8 || divice_bit == 9) {
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 1, 9);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[0]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            mSleep(2000);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[1]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            mSleep(2000);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[2]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            mSleep(2000);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[3]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            mSleep(2000);


        } else if (divice_bit == 16 || divice_bit == 17) {
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 1, 17);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[0]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[1]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[2]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[3]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
        }
        lees = (lees + 8);

    }else if(lees == schrijf){
        I2CTWI_transmitByte(I2C_RP6_BASE_ADR, 27); 
        uint8_t divice_bit = 8;// I2CTWI_readByte(I2C_RP6_BASE_ADR);

        transmit_buffer[0]= SPI_EEPROM_readByte(lees);
        transmit_buffer[1]= SPI_EEPROM_readByte(lees+1);
        transmit_buffer[2]= SPI_EEPROM_readByte(lees+2);
        transmit_buffer[3]= SPI_EEPROM_readByte(lees+3);
        mSleep(10);

        if (divice_bit == 8 || divice_bit == 9) {
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 1, 9);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[0]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            mSleep(2000);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[1]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            mSleep(2000);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[2]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            mSleep(2000);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[3]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            mSleep(2000);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, 50);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);

        } else if (divice_bit == 16 || divice_bit == 17) {
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 1, 17);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[0]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[1]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[2]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, transmit_buffer[3]);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 2, 84);
            I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, 10);
        }	
        lees = (lees + 8);
    }
}

void ir_receiveBaseStation(void)
{
    while ((personX == 0xFF) || (personY == 0xFF) || (startX == 0xFF) || (startY == 0xFF)) {
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

        // for now, just put it on the screen
        clearLCD();

        setCursorPosLCD(0, 0);
        writeCharLCD('X');

        setCursorPosLCD(0, 2);
        writeIntegerLCD(startX, DEC);

        setCursorPosLCD(0, 10);
        writeCharLCD('Y');

        setCursorPosLCD(0, 12);
        writeIntegerLCD(startY, DEC);


        setCursorPosLCD(1, 0);
        writeCharLCD('X');

        setCursorPosLCD(1, 2);
        writeIntegerLCD(personX, DEC);

        setCursorPosLCD(1, 10);
        writeCharLCD('Y');

        setCursorPosLCD(1, 12);
        writeIntegerLCD(personY, DEC);
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

    SPI_EEPROM_writeByte(schrijf,receive_buffer[0]);
    mSleep(10);
    SPI_EEPROM_writeByte(schrijf+1,receive_buffer[1]);
    mSleep(10);
    SPI_EEPROM_writeByte(schrijf+2,receive_buffer[2]);
    mSleep(10);
    SPI_EEPROM_writeByte(schrijf+3,receive_buffer[3]);
    mSleep(10);
    schrijf = (schrijf + 8);
}


void ir_init(void)
{
    receive_buffer[0] = 2;
    receive_buffer[1] = 17;
    receive_buffer[2] = 32;
    receive_buffer[3] = 45;
    receive_buffer[4] = 7;
    receive_buffer[5] = 23;
    receive_buffer[6] = 33;
    receive_buffer[7] = 41;


    SPI_EEPROM_writeByte(schrijf, receive_buffer[0]);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+1, receive_buffer[1]);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+2, receive_buffer[2]);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+3, receive_buffer[3]);
    mSleep(50);
    schrijf = (schrijf + 8);

    SPI_EEPROM_writeByte(schrijf, receive_buffer[4]);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+1, receive_buffer[5]);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+2, receive_buffer[6]);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+3, receive_buffer[7]);
    mSleep(50);
    schrijf = (schrijf + 8);

    ir_sendSituation(NORTH, 1, 2);


    SPI_EEPROM_writeByte(schrijf, 5);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+1, 21);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+2, 34);
    mSleep(50);
    SPI_EEPROM_writeByte(schrijf+3, 47);
    mSleep(50);
}
