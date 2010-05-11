/* ****************************************************************************
 *                           _______________________
 *                           \| RP6  ROBOT SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/         >>> BASE CONTROLLER
 * ----------------------------------------------------------------------------
 * ------------------- [c]2006 / 2007 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * ****************************************************************************
 * File: RP6I2CslaveTWI.c
 * Version: 1.0
 * Target: RP6 Base & Processor Expansion - ATMEGA32 @8.00 or 16.00MHz
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * I2C Slave driver - completely interrupt based! It offers registers that
 * can be read and written - just like other I2C Devices.
 *
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */
 
/*****************************************************************************/
// Includes:

#include "RP6I2CslaveTWI.h"

/*****************************************************************************/

/**
 * This function initializes the TWI Module to Slave Mode. You can pass slave address and
 * general call settings directly in the address byte.
 *
 * Example:
 * I2CTWI_initSlave((TWI_slaveAddress<<TWI_ADR_BITS) | (TRUE<<TWI_GEN_BIT));  
 */
void I2CTWI_initSlave(uint8_t address)
{
	cli();
	TWAR = address;                  // Set own TWI slave address. Accept TWI General Calls.
	TWDR = 0xFF;                     // Default content = SDA released.
	TWCR = (1<<TWEN);
	sei();
	// Start the TWI transceiver to enable reception of the first command from the TWI Master.
	TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
}


/*****************************************************************************/
// TWI ISR:

#define I2CTWI_STATE_IDLE 0
#define I2CTWI_STATE_GEN_CALL 1
#define I2CTWI_STATE_WRITE_REG 2
#define I2CTWI_STATE_WRITE_DATA 3
#define I2CTWI_STATE_READ_REG 4
#define I2CTWI_STATE_REP_START 5


uint8_t I2CTWI_readRegisters[I2CTWI_SLAVE_READ_REGISTERS];
volatile uint8_t I2CTWI_writeRegisters[I2CTWI_SLAVE_WRITE_REGISTERS];
volatile uint8_t I2CTWI_genCallCMD;
volatile uint8_t I2CTWI_dataWasRead = 0;
volatile uint8_t I2CTWI_dataReadFromReg = 0;

volatile uint8_t I2CTWI_readBusy = 0;
volatile uint8_t I2CTWI_writeBusy = 0;

ISR (TWI_vect)
{
	static uint8_t current_register = 0;
	static uint8_t TWI_state = I2CTWI_STATE_IDLE;
	switch (TWSR) {
		case TWI_STX_ADR_ACK:   // Own SLA+R has been received; ACK has been returned
			TWI_state = I2CTWI_STATE_READ_REG;				
			I2CTWI_readBusy = 1;		
		case TWI_STX_DATA_ACK:  // Data byte in TWDR has been transmitted; ACK has been received
			if(TWI_state == I2CTWI_STATE_READ_REG)
				TWDR = I2CTWI_readRegisters[current_register++];
			TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA); // Enable TWI Interupt and clear the flag to send byte
		break;
		case TWI_STX_DATA_NACK: // Data byte in TWDR has been transmitted; NACK has been received.
			TWI_state = I2CTWI_STATE_IDLE; // ... this is most likely the end of the transmission.
			current_register = 0;
			I2CTWI_readBusy = 0;	
			I2CTWI_dataWasRead = 1;
			TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
		break;
		case TWI_SRX_ADR_ACK: // Own SLA+W has been received ACK has been returned
			I2CTWI_writeBusy = 1;
			TWI_state = I2CTWI_STATE_WRITE_REG;	
			TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA); // Reset the TWI Interupt to wait for a new event.	
		break;
		case TWI_SRX_ADR_DATA_ACK: // Previously addressed with own SLA+W; data has been received; ACK has been returned
			if(TWI_state == I2CTWI_STATE_WRITE_REG) {
				current_register = TWDR;
				I2CTWI_dataReadFromReg = current_register;
				TWI_state = I2CTWI_STATE_WRITE_DATA;
			}
			else if(TWI_state == I2CTWI_STATE_WRITE_DATA)
				I2CTWI_writeRegisters[current_register++] = TWDR;
			TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA); // Reset the TWI Interupt to wait for a new event, send ACK after next reception
		break;
		case TWI_SRX_GEN_ACK: // General call address has been received; ACK has been returned
			TWI_state = I2CTWI_STATE_GEN_CALL;
			TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
		break;
		case TWI_SRX_GEN_DATA_ACK: // Previously addressed with general call; data has been received; ACK has been returned
			I2CTWI_genCallCMD = TWDR;
			TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA); // Send ACK after next reception
	    break;
		case TWI_SRX_STOP_RESTART:  // A STOP condition or repeated START condition has been received while still addressed as Slave
			TWI_state = I2CTWI_STATE_REP_START;
			TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
			I2CTWI_writeBusy = 0;
		break;
		default: // Any error, no state etc. 
			TWCR = (1<<TWEN)|(1<<TWINT); 
		break;
	}
}

/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * - v. 1.0 (initial release) 16.05.2007 by Dominik S. Herwald
 *
 * ****************************************************************************
 * Bugs, feedback, questions and modifications can be posted on the AREXX Forum
 * on http://www.arexx.com/forum/ !
 * Of course you can also write us an e-mail to: info@arexx.nl
 * AREXX Engineering may publish updates from time to time on AREXX.com!
 * ****************************************************************************
 * - LICENSE -
 * GNU GPL v2 (http://www.gnu.org/licenses/gpl.txt, a local copy can be found
 * on the RP6 CD in the RP6 sorce code folders!)
 * This program is free software. You can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 * ****************************************************************************
 */

/*****************************************************************************/
// EOF
