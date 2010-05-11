/* ****************************************************************************
 *                           _______________________
 *                           \| RP6  ROBOT SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/         >>> BASE CONTROLLER
 * ----------------------------------------------------------------------------
 * ------------------- [c]2006 / 2007 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * ****************************************************************************
 * File: RP6RobotBase.h
 * Version: 1.01
 * Target: RP6 Base - ATMEGA32 @8.00MHz
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * The RP6 Robot Base header file with general definitions. If you don't want
 * to include the complete RP6BaseLib because it is too large - then at least
 * include this file! It is already included in the RP6BaseLib, but can be
 * used without it!
 *
 * This file contains helpful definitions that simplify reading the sourcecode.
 * Most important are the default settings for Port and Direction registers!
 * Hint: You should better leave all this as it is, but it is a good idea to
 * read the comments, it will help you to understand C programming for AVR
 * better!
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */

#ifndef RP6BASE_H
#define RP6BASE_H

/*****************************************************************************/
// Useful definition for common routines that are used on several
// different devices (can switch register settings to match the target!):

#define DEVICE_RP6BASE

/*****************************************************************************/
// CPU Clock:

#ifdef F_CPU
#undef F_CPU
#endif
#ifndef F_CPU
#define F_CPU 8000000 //Base: 8.00MHz  - DO NOT CHANGE!
#endif

/*****************************************************************************/
// Includes:

#include <avr/io.h>			// I/O Port definitions
#include <avr/interrupt.h>	// Interrupt macros (e.g. cli(), sei())

/*****************************************************************************/
// I/O PORT pin definitions
// These definitions simplify reading and understanding the source code.
//
// ATTENTION: Initial value of port and direction registers should not
// be changed, if you do not exactly know what you are doing!
//
// Hints for DDRx and PORTx Registers:
// DDRxy = 0 and PORTxy = 0 ==> Input without internal Pullup
// DDRxy = 0 and PORTxy = 1 ==> Input with internal Pullup
// DDRxy = 1 and PORTxy = 0 ==> Output low
// DDRxy = 1 and PORTxy = 1 ==> Output high
// "=1" indicates that the appropriate bit is set.
//
// Example:
// #define INIT_DDRA 0b00010000
// #define INIT_PRTA 0b00000000
//
// This means that ALL ports on PortA are inputs without internal pullups
// except for PortA4, which is an output (E_INT1 signal in this case) and
// initial value is low.
//
// Binary value explanation:
// 0b00010000     = 16 in decimal system
//   ^      ^
// MSB      LSB      (MSB = Most Significant Bit, LSB = Least Significant Bit)
//
// The program should always call the macro "portInit();" FIRST! You can find
// it a bit below. Correct port initialisation is the most important step
// after a hardware reset!

// ---------------------------------------------------
// PORTA

#define UBAT 			(1 << PINA7) // ADC7 (Input)
#define MCURRENT_L 		(1 << PINA6) // ADC6 (Input)
#define MCURRENT_R 		(1 << PINA5) // ADC5 (Input)
#define E_INT1 			(1 << PINA4) // INT1 (input per default... can be output)
#define LS_L 			(1 << PINA3) // ADC3 (Input)
#define LS_R 			(1 << PINA2) // ADC2 (Input)
#define ADC1 			(1 << PINA1) // ADC1 (Input)
#define ADC0 			(1 << PINA0) // ADC0 (Input)

// Initial value of port and direction registers.
#define INIT_DDRA 0b00000000
#define INIT_PRTA 0b00000000

// ---------------------------------------------------
// PORTA A/D Convertor channels

#define ADC_BAT 			7
#define ADC_MCURRENT_L 		6
#define ADC_MCURRENT_R 		5
#define ADC_LS_L 			3
#define ADC_LS_R 			2
#define ADC_ADC1 			1
#define ADC_ADC0 			0

// ---------------------------------------------------
// PORTB

#define SL4 		(1 << PINB7)	// Output
#define ACS_L 		(1 << PINB6)	// Output
#define START 		(1 << PINB5)	// Input
#define PWRON		(1 << PINB4)	// Output
#define ACS_PWRH	(1 << PINB3)	// Output
#define ACS 		(1 << PINB2)	// INT2 (Input)
#define SL5 		(1 << PINB1)	// Output
#define SL6 		(1 << PINB0)	// Output

// Initial value of port and direction registers.
#define INIT_DDRB 0b01011000
#define INIT_PRTB 0b00000000

// ---------------------------------------------------
// PORTC

#define ACS_R 		(1 << PINC7)	// Output
#define SL3 		(1 << PINC6)	// Output
#define SL2 		(1 << PINC5)	// Output
#define SL1 		(1 << PINC4)	// Output
#define DIR_R 		(1 << PINC3)	// Output
#define DIR_L 		(1 << PINC2)	// Output
#define SDA 		(1 << PINC1)	// I2C Data (I/O)
#define SCL 		(1 << PINC0)	// I2C Clock (Output (Master), Input (Slave))

// Initial value of port and direction registers.
#define INIT_DDRC 0b10001100
#define INIT_PRTC 0b00000000

// ---------------------------------------------------
// PORTD

#define ACS_PWR	(1 << PIND6)	// Output
#define MOTOR_R	(1 << PIND5)	// PWM Output (OC1A)
#define MOTOR_L	(1 << PIND4)	// PWM Output (OC1B)
#define ENC_R 	(1 << PIND3)	// INT1 (Input)
#define ENC_L 	(1 << PIND2)	// INT0 (Input)
#define TX 		(1 << PIND1)	// USART TX (Output)
#define RX 		(1 << PIND0)	// USART RX (Input)

// Initial value of port and direction registers.
#define INIT_DDRD 0b11110010
#define INIT_PRTD 0b00000001


/*****************************************************************************/
// IRCOMM pin:

// ### WARNING! 
// 
// #define IRCOMM 	(1 << PIND7)	// Output
//
// ### DO NOT USE THIS PIN BY YOURSELF! 
// ONLY LET THE INTERRUPT ROUTINE OF THE LIBRARY 
// CONTROL THIS PIN! 
// The IR LEDs must be controlled by a modulated
// signal with minimal 5kHz or higher 
// modulation frequency! 
// Nominal modulation frequency is 36kHz! 
// YOU MAY DAMAGE THE IRCOMM IF YOU USE IT 
// IN ANY OTHER WAY! 

// Only use this macro to make sure IRCOMM is
// turned off:
#define IRCOMM_OFF() PORTD &= ~(1 << PIND7);

// NEVER try to control IRCOMM by yourself if you
// do not know what you are doing!
// Always use Timer based modulation control!


/*****************************************************************************/
// I/O Port init macro - always call this first! It is called first from
// initRobotBase() in the RP6RobotBaseLib!
//
// Example:
// int main(void)
// {
// 		portInit();
// 		// ...
//		// your application
//		while(true);
//		return 0;
// }

#define portInit();	\
PORTA = INIT_PRTA;	\
PORTB = INIT_PRTB;	\
PORTC = INIT_PRTC;	\
PORTD = INIT_PRTD;	\
DDRA = INIT_DDRA;	\
DDRB = INIT_DDRB;	\
DDRC = INIT_DDRC;	\
DDRD = INIT_DDRD;

/*****************************************************************************/
// Some additional definitions/macros

// Boolean:
#define true 1
#define false 0
#define TRUE 1
#define FALSE 0

// Assembly and system macros:
#define nop() asm volatile("nop\n\t")
#define sysSleep() asm volatile("sleep\n\t")

// Enable/Disable Encoders, IR Receiver and Current Sensors to
// save power when not moving.
#define __powerON() PORTB |= PWRON
#define __powerOFF() PORTB &= ~PWRON

// Encoders:
#define isEncoderLeft()  (PIND & ENC_L)
#define isEncoderRight() (PIND & ENC_R)

// Set the Start/Stop Button action to reset (=Stop) the controller (and all
// other expansion controllers connected to the main reset line!):
#define enableResetButton(); ({PORTB &= ~START; DDRB |= START;})
// Set the Start/Stop Button to act as the start button:
#define disableResetButton(); ({PORTB &= ~START; DDRB &= ~START;})
// Check if start button is pressed:
#define isStartButton() (PINB & START)

/*****************************************************************************/
// Baudrates:

#define BAUD_LOW		38400  //Low speed - 38.4 kBaud
#define UBRR_BAUD_LOW	((F_CPU/(16*BAUD_LOW))-1)

#define BAUD_HIGH		500000 //High speed - 500 kBaud
#define UBRR_BAUD_HIGH	((F_CPU/(16*BAUD_HIGH))-1)

#endif

/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * - v. 1.01 30.07.2007 by Dominik S. Herwald
 * 		- removed IRCOMM pin definition and added IRCOMM_OFF() macro instead.
 * - v. 1.0 (initial release) 10.04.2007 by Dominik S. Herwald
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
