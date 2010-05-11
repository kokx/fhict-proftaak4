/* ****************************************************************************
 *                           _______________________
 *                           \| RP6  ROBOT SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/         >>> BASE CONTROLLER
 * ----------------------------------------------------------------------------
 * ------------------- [c]2006 / 2007 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * ****************************************************************************
 * File: RP6RobotBaseLib.h
 * Version: 1.4
 * Target: RP6 Base - ATMEGA32 @8.00MHz
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * The RP6 Robot Base function Library header file. Detailled description
 * of each function can be found in the RP6RobotBaseLib.c file!
 *
 *
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */

#ifndef RP6BASELIB_H
#define RP6BASELIB_H

/*****************************************************************************/
// Includes:

#include "RP6RobotBase.h"	// General RP6 Robot Base definitions
#include "RP6Config.h"      // Configuration for RP6BaseLibrary - Constants for 
							// speed and distance calculation etc.
#include "RP6uart.h"		// RP6 UART function lib

#include <avr/sleep.h>		// Power saving functions
#include <stdlib.h>


/*****************************************************************************/

// You can use this define to identify the RP6Library version:
#define RP6LIB_VERSION 13
// (this does not mean file version - each file has its own
// sub-version but the complete library has an overall version)

/*****************************************************************************/
// Init:

void initRobotBase(void);

/*****************************************************************************/
// Status LEDs

// A shadow register that simplifies usage of status LEDs:
union {
 	uint8_t byte;
	struct {
		unsigned LEDsR:3;
		unsigned LEDsL:3;
		unsigned reserved:2;
	};
	struct {
		unsigned LED1:1;
		unsigned LED2:1;
		unsigned LED3:1;
		unsigned LED4:1;
		unsigned LED5:1;
		unsigned LED6:1;
		unsigned reserved1:1;
		unsigned reserved2:1;
	};
} statusLEDs;

void updateStatusLEDs(void);
void setLEDs(uint8_t leds);

#ifdef POWER_ON_WARNING
	void enablePowerOnWarning(void);
	void disablePowerOnWarning(void);
#endif

/*****************************************************************************/
// Bumpers

uint8_t getBumperLeft(void);
uint8_t getBumperRight(void);
extern uint8_t bumper_left;
extern uint8_t bumper_right;
#define isBumperLeft() (bumper_left)
#define isBumperRight() (bumper_right)
void BUMPERS_setStateChangedHandler(void (*bumperHandler)(void)); 
void task_Bumpers(void);

/*****************************************************************************/
// ADC

extern uint8_t adc_conversion_complete;
extern uint16_t adc_result;

extern uint16_t adcBat;
extern uint16_t adcMotorCurrentLeft;
extern uint16_t adcMotorCurrentRight;
extern uint16_t adcLSL;
extern uint16_t adcLSR;
extern uint16_t adc0;
extern uint16_t adc1;

uint16_t readADC(uint8_t channel);
void startADC(uint8_t channel);
void task_ADC(void);

/*****************************************************************************/
// Motor control

// Motion Control status register:
union {
	uint8_t byte;
	struct {
		unsigned move_L:1;
		unsigned move_R:1;
		unsigned overcurrent:1;
		unsigned reserved:5;
	};
} motion_status;

extern uint8_t mleft_dir;
extern uint8_t mright_dir;
extern int16_t mleft_power;
extern int16_t mright_power;

#define MC_IDLE 0
#define MC_MOVEDIST_R 1
#define MC_MOVEDIST_L 2
#define MC_ROTATE 4

#define MC_CHANGE_DIR_R 32
#define MC_CHANGE_DIR_L 64

void setMotorDir(uint8_t left_dir, uint8_t right_dir);

/**
 * These Macros return the current direction value. 
 * They return true if direction is Forwards, false otherwise.
 */
#define getLeftDir() (mleft_dir)
#define getRightDir() (mright_dir)

//Speed:
void setMotorPower(uint8_t left_power, uint8_t right_power);

/**
 * You can use these two Macros to get the current speed value.
 * They are just synonyms for the two OCR1x Registers of the Timer Module.
 */
#define getMotorRight() (OCR1A)
#define getMotorLeft() (OCR1B)

//Encoders:
extern volatile uint16_t mleft_speed;
extern volatile uint16_t mright_speed;

#define getLeftSpeed() (mleft_speed)
#define getRightSpeed() (mright_speed)

// Distance
extern volatile uint16_t mleft_dist;
extern volatile uint16_t mright_dist;

#define getLeftDistance() (mleft_dist)
#define getRightDistance() (mright_dist)

// Desired Speed:
extern uint16_t mleft_des_speed;
extern uint16_t mright_des_speed;

#define getDesSpeedLeft() (mleft_des_speed)
#define getDesSpeedRight() (mright_des_speed)

//Direction:
#define FWD 0
#define BWD 1
#define LEFT 2
#define RIGHT 3

extern uint8_t drive_dir;
#define getDirection() (drive_dir)
void changeDirection(uint8_t dir);

#define NON_BLOCKING 0
#define BLOCKING 1

void move(uint8_t desired_speed, uint8_t dir, uint16_t distance, uint8_t blocking);
void rotate(uint8_t desired_speed, uint8_t dir, uint16_t angle, uint8_t blocking);
void task_motionControl(void);
void moveAtSpeed(uint8_t desired_speed_left, uint8_t desired_speed_right);
uint8_t isMovementComplete(void);
void stop(void);
void MOTIONCONTROL_setStateChangedHandler(void (*motionControlHandler)(void));

#ifdef DEBUG_MEASURE_DUTY_CYCLE
	extern volatile uint16_t cycle_h_l;
	extern volatile uint16_t cycle_l_l;
	extern volatile uint16_t cycle_h_r;
	extern volatile uint16_t cycle_l_r;
#endif

/*****************************************************************************/
// ACS

extern uint8_t obstacle_left;
extern uint8_t obstacle_right;
#define isObstacleLeft() (obstacle_left)
#define isObstacleRight() (obstacle_right)

void disableACS(void);
void enableACS(void);

void setACSPwrOff(void);
void setACSPwrLow(void);
void setACSPwrMed(void);
void setACSPwrHigh(void);

void task_ACS(void);

void ACS_setStateChangedHandler(void (*acsHandler)(void));

void powerON(void);
void powerOFF(void);

/*****************************************************************************/
// RC5:

#define TOGGLEBIT 32

typedef union {
	uint16_t data;
	struct {
		unsigned key_code:6;
		unsigned device:5;
		unsigned toggle_bit:1;
		unsigned reserved:3;
	};
} RC5data_t;

void IRCOMM_sendRC5(uint8_t adr, uint8_t data);
void IRCOMM_setRC5DataReadyHandler(void (*rc5Handler)(RC5data_t));

/*****************************************************************************/
// Stopwatches:

#define STOPWATCH1 1
#define STOPWATCH2 2
#define STOPWATCH3 4
#define STOPWATCH4 8
#define STOPWATCH5 16
#define STOPWATCH6 32
#define STOPWATCH7 64
#define STOPWATCH8 128

/*
 * 16bit Stopwatches --> maximum value is 65535 or ~65.535 seconds!
 */
typedef struct {
	volatile uint8_t watches;
	volatile uint16_t watch1;
	volatile uint16_t watch2;
	volatile uint16_t watch3;
	volatile uint16_t watch4;
	volatile uint16_t watch5;
	volatile uint16_t watch6;
	volatile uint16_t watch7;
	volatile uint16_t watch8;
} stopwatches_t;
extern volatile stopwatches_t stopwatches;

#define stopStopwatch1() stopwatches.watches &= ~STOPWATCH1
#define stopStopwatch2() stopwatches.watches &= ~STOPWATCH2
#define stopStopwatch3() stopwatches.watches &= ~STOPWATCH3
#define stopStopwatch4() stopwatches.watches &= ~STOPWATCH4
#define stopStopwatch5() stopwatches.watches &= ~STOPWATCH5
#define stopStopwatch6() stopwatches.watches &= ~STOPWATCH6
#define stopStopwatch7() stopwatches.watches &= ~STOPWATCH7
#define stopStopwatch8() stopwatches.watches &= ~STOPWATCH8

#define startStopwatch1() stopwatches.watches |= STOPWATCH1
#define startStopwatch2() stopwatches.watches |= STOPWATCH2
#define startStopwatch3() stopwatches.watches |= STOPWATCH3
#define startStopwatch4() stopwatches.watches |= STOPWATCH4
#define startStopwatch5() stopwatches.watches |= STOPWATCH5
#define startStopwatch6() stopwatches.watches |= STOPWATCH6
#define startStopwatch7() stopwatches.watches |= STOPWATCH7
#define startStopwatch8() stopwatches.watches |= STOPWATCH8

#define isStopwatch1Running() (stopwatches.watches & STOPWATCH1)
#define isStopwatch2Running() (stopwatches.watches & STOPWATCH2)
#define isStopwatch3Running() (stopwatches.watches & STOPWATCH3)
#define isStopwatch4Running() (stopwatches.watches & STOPWATCH4)
#define isStopwatch5Running() (stopwatches.watches & STOPWATCH5)
#define isStopwatch6Running() (stopwatches.watches & STOPWATCH6)
#define isStopwatch7Running() (stopwatches.watches & STOPWATCH7)
#define isStopwatch8Running() (stopwatches.watches & STOPWATCH8)

#define getStopwatch1() stopwatches.watch1
#define getStopwatch2() stopwatches.watch2
#define getStopwatch3() stopwatches.watch3
#define getStopwatch4() stopwatches.watch4
#define getStopwatch5() stopwatches.watch5
#define getStopwatch6() stopwatches.watch6
#define getStopwatch7() stopwatches.watch7
#define getStopwatch8() stopwatches.watch8

#define setStopwatch1(__VALUE__) stopwatches.watch1 = __VALUE__
#define setStopwatch2(__VALUE__) stopwatches.watch2 = __VALUE__
#define setStopwatch3(__VALUE__) stopwatches.watch3 = __VALUE__
#define setStopwatch4(__VALUE__) stopwatches.watch4 = __VALUE__
#define setStopwatch5(__VALUE__) stopwatches.watch5 = __VALUE__
#define setStopwatch6(__VALUE__) stopwatches.watch6 = __VALUE__
#define setStopwatch7(__VALUE__) stopwatches.watch7 = __VALUE__
#define setStopwatch8(__VALUE__) stopwatches.watch8 = __VALUE__


/*****************************************************************************/
// Universal timer:

// This timer variable:
volatile uint16_t timer;  
// is incremented each 100µs. It is suited for performing time
// measurements where the resolution of the stopwatches (1ms) is not enough. 

/*****************************************************************************/
// Delays:

void sleep(uint8_t time);
void mSleep(uint16_t time);
void delayCycles(uint16_t dly);
#define delay_us(us)  _delay_loop_2(( ( ( 1*(F_CPU/4000) )*us )/1000 ))

/*****************************************************************************/

void extIntON(void);
void extIntOFF(void);

/*****************************************************************************/

void task_RP6System(void);

/*****************************************************************************/

#endif

/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * 
 *  ---> changes are documented in the file "RP6RobotBaseLib.c"
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
