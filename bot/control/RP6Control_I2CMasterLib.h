/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master 9 - Lib 2
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * Header file for new library.
 *
 * ****************************************************************************
 */

#ifndef RP6CONTROL_I2CMASTERLIB_H
#define RP6CONTROL_I2CMASTERLIB_H


/*****************************************************************************/
// Includes:

#include "RP6ControlLib.h"
#include "RP6I2CmasterTWI.h"

// define the RP6 Base address here:
#define I2C_RP6_BASE_ADR 10

/*****************************************************************************/
// These are the same command definitions as you can find them in the 
// I2C Bus Slave Example program for RP6Base:

#define I2C_REG_STATUS1 		 0
#define I2C_REG_STATUS2 		 1
#define I2C_REG_MOTION_STATUS 	 2
#define I2C_REG_POWER_LEFT 		 3
#define I2C_REG_POWER_RIGHT 	 4
#define I2C_REG_SPEED_LEFT 		 5
#define I2C_REG_SPEED_RIGHT 	 6
#define I2C_REG_DES_SPEED_LEFT 	 7
#define I2C_REG_DES_SPEED_RIGHT  8
#define I2C_REG_DIST_LEFT_L 	 9
#define I2C_REG_DIST_LEFT_H 	 10
#define I2C_REG_DIST_RIGHT_L     11
#define I2C_REG_DIST_RIGHT_H 	 12
#define I2C_REG_ADC_LSL_L 		 13
#define I2C_REG_ADC_LSL_H 		 14
#define I2C_REG_ADC_LSR_L 		 15
#define I2C_REG_ADC_LSR_H 		 16
#define I2C_REG_ADC_MOTOR_CURL_L 17
#define I2C_REG_ADC_MOTOR_CURL_H 18
#define I2C_REG_ADC_MOTOR_CURR_L 19
#define I2C_REG_ADC_MOTOR_CURR_H 20
#define I2C_REG_ADC_UBAT_L 		 21
#define I2C_REG_ADC_UBAT_H 		 22
#define I2C_REG_ADC_ADC0_L 		 23
#define I2C_REG_ADC_ADC0_H 		 24
#define I2C_REG_ADC_ADC1_L 		 25
#define I2C_REG_ADC_ADC1_H 		 26
#define I2C_REG_RC5_ADR	 		 27
#define I2C_REG_RC5_DATA	 	 28
#define I2C_REG_LEDS	 		 29

#define CMD_POWER_OFF 		0
#define CMD_POWER_ON 		1
#define CMD_CONFIG 			2
#define CMD_SETLEDS 		3
#define CMD_STOP   			4
#define CMD_MOVE_AT_SPEED   5
#define CMD_CHANGE_DIR	    6
#define CMD_MOVE 			7
#define CMD_ROTATE 			8
#define CMD_SET_ACS_POWER	9 
#define CMD_SEND_RC5		10 
#define CMD_SET_WDT			11
#define CMD_SET_WDT_RQ		12

#define ACS_PWR_OFF  0
#define ACS_PWR_LOW  1
#define ACS_PWR_MED  2
#define ACS_PWR_HIGH 3


/*****************************************************************************/

union {
 	uint8_t byte;
	struct {
		uint8_t batLow:1;
		uint8_t bumperLeft:1;
		uint8_t bumperRight:1;
		uint8_t RC5reception:1;
		uint8_t RC5transmitReady:1;
		uint8_t obstacleLeft:1;
		uint8_t obstacleRight:1;
		uint8_t driveSystemChange:1;
	};
} interrupt_status;

union {
 	uint8_t byte;
	struct {
		uint8_t powerOn:1;
		uint8_t ACSactive:1;
		uint8_t watchDogTimer:1;
		uint8_t wdtRequest:1;
		uint8_t unused:4;
	};
} status;

union {
 	uint8_t byte;
	struct {
		uint8_t movementComplete:1;
		uint8_t motorsOn:1;
		uint8_t motorOvercurrent:1;
		uint8_t direction:2;
		uint8_t unused:3;
	};
} drive_status;

/*****************************************************************************/

#define getMotorRight() (OCR1A)
#define getMotorLeft() (OCR1B)

//Encoders:
extern uint8_t mleft_speed;
extern uint8_t mright_speed;

// Distance
extern uint16_t mleft_dist;
extern uint16_t mright_dist;

// Desired Speed:
extern uint8_t mleft_des_speed;
extern uint8_t mright_des_speed;

// Power
extern uint8_t mleft_power;
extern uint8_t mright_power;

#define getLeftSpeed() (mleft_speed)
#define getRightSpeed() (mright_speed)
#define getLeftDistance() (mleft_dist)
#define getRightDistance() (mright_dist)

#define getDesSpeedLeft() (mleft_des_speed)
#define getDesSpeedRight() (mright_des_speed)

//Direction:

#define FWD 0
#define BWD 1
#define LEFT 2
#define RIGHT 3

#define getDirection() (drive_status.direction)


#define INT0_STATUS_CHECK 0



/*****************************************************************************/
// 
void initI2C_RP6Lib(void);
uint8_t checkRP6Status(uint8_t dataRequestID);

void I2C_transmissionError(uint8_t errorState);

void task_checkINT0(void);

#define NON_BLOCKING 0
#define BLOCKING 1

void rotate(uint8_t desired_speed, uint8_t dir, uint16_t angle, uint8_t blocking);
void move(uint8_t desired_speed, uint8_t dir, uint16_t distance, uint8_t blocking);
void moveAtSpeed(uint8_t desired_speed_left, uint8_t desired_speed_right);
void changeDirection(uint8_t dir);
void stop(void);

#define isMovementComplete() (drive_status.movementComplete)

	
extern uint8_t bumper_left;
extern uint8_t bumper_right;

extern uint8_t obstacle_left;
extern uint8_t obstacle_right;
#define isObstacleLeft() (obstacle_left)
#define isObstacleRight() (obstacle_right)

extern uint16_t adcBat;
extern uint16_t adcMotorCurrentLeft;
extern uint16_t adcMotorCurrentRight;
extern uint16_t adcLSL;
extern uint16_t adcLSR;
extern uint16_t adc0;
extern uint16_t adc1;

#define TOGGLEBIT 32

typedef union {
	uint16_t data;
	struct {
		uint8_t key_code:6;
		uint8_t device:5;
		uint8_t toggle_bit:1;
		uint8_t reserved:3;
	};
} RC5data_t;


void IRCOMM_sendRC5(uint8_t adr, uint8_t data);

void IRCOMM_setRC5DataReadyHandler(void (*rc5Handler)(RC5data_t));
void ACS_setStateChangedHandler(void (*acsHandler)(void));
void MOTIONCONTROL_setStateChangedHandler(void (*motionControlHandler)(void));
void BUMPERS_setStateChangedHandler(void (*bumperHandler)(void));
void BATTERY_setLowVoltageHandler(void (*lowVoltageHandler)(uint8_t));
void WDT_setRequestHandler(void (*requestHandler)(void));

void getAllSensors(void);

#endif

