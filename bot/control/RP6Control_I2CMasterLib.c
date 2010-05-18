/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Example: I2C Master 9 - Lib 2
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 * 
 * We added some movement functions to the library in this example!
 *
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6Control_I2CMasterLib.h" 		

/*****************************************************************************/
// Bumper:

uint8_t bumper_left;
uint8_t bumper_right;

// -------------------------------
// Bumpers State changed handler:

void BUMPERS_stateChanged_DUMMY(void){}
static void (*BUMPERS_stateChangedHandler)(void) = BUMPERS_stateChanged_DUMMY;
/**
 * Use this function to set the Bumpers state change handler. 
 * 
 */
void BUMPERS_setStateChangedHandler(void (*bumperHandler)(void)) 
{
	BUMPERS_stateChangedHandler = bumperHandler;
}

/*****************************************************************************/
// MotionControl:

// -------------------------------
// MotionControl state changed handler:

void MOTIONCONTROL_stateChanged_DUMMY(void){}
static void (*MOTIONCONTROL_stateChangedHandler)(void) = MOTIONCONTROL_stateChanged_DUMMY;

void MOTIONCONTROL_setStateChangedHandler(void (*motionControlHandler)(void)) 
{
	MOTIONCONTROL_stateChangedHandler = motionControlHandler;
}

/*****************************************************************************/
// RC5:

// -------------------------------
// RC5 Data reception Handler:

void IRCOMM_RC5dataReady_DUMMY(RC5data_t rc5data){}
static void (*IRCOMM_RC5dataReadyHandler)(RC5data_t rc5data) = IRCOMM_RC5dataReady_DUMMY;

void IRCOMM_setRC5DataReadyHandler(void (*rc5Handler)(RC5data_t rc5data)) 
{
	IRCOMM_RC5dataReadyHandler = rc5Handler;
}

/*****************************************************************************/
// ACS:

uint8_t obstacle_left;
uint8_t obstacle_right;

// -------------------------------
// ACS State changed handler:

void ACS_stateChanged_DUMMY(void){}
static void (*ACS_stateChangedHandler)(void) = ACS_stateChanged_DUMMY;

void ACS_setStateChangedHandler(void (*acsHandler)(void)) 
{
	ACS_stateChangedHandler = acsHandler;
}

/*****************************************************************************/
// Battery Voltage Sensor:

// -------------------------------
// Low Battery Voltage Event Handler:

void BATTERY_lowVoltage_DUMMY(uint8_t isVoltageLow){}
static void (*BATTERY_lowVoltageHandler)(uint8_t) = BATTERY_lowVoltage_DUMMY;

void BATTERY_setLowVoltageHandler(void (*lowVoltageHandler)(uint8_t)) 
{
	BATTERY_lowVoltageHandler = lowVoltageHandler;
}


/*****************************************************************************/
// WDT:

// -------------------------------
// WDT Request Event Handler:

void WDT_request_DUMMY(void){}
static void (*WDT_requestHandler)(void) = WDT_request_DUMMY;

void WDT_setRequestHandler(void (*requestHandler)(void)) 
{
	WDT_requestHandler = requestHandler;
}

/*****************************************************************************/
// Sensors/ADCs: 

// ADCs:
uint16_t adcBat;
uint16_t adcMotorCurrentLeft;
uint16_t adcMotorCurrentRight;
uint16_t adcLSL;
uint16_t adcLSR;
uint16_t adc0;
uint16_t adc1;

// Measured Speed:
uint8_t mleft_speed;
uint8_t mright_speed;

// Distance
uint16_t mleft_dist;
uint16_t mright_dist;

// Desired Speed:
uint8_t mleft_des_speed;
uint8_t mright_des_speed;

// Power
uint8_t mleft_power;
uint8_t mright_power;

uint8_t sensorBuf[24]; 

/**
 * In order to use the same register names as in the RP6Lib, this
 * function reads all ADC channels and all motor parameters into
 * the same values as in the RP6Lib. 
 * Of course this function needs some time to read all these
 * 24 registers via the I2C Bus. 
 */
void getAllSensors(void)
{
	I2CTWI_readRegisters(I2C_RP6_BASE_ADR, I2C_REG_POWER_LEFT, sensorBuf, 24);
	mleft_power = sensorBuf[0];
	mright_power = sensorBuf[1];
	mleft_speed = sensorBuf[2];
	mright_speed = sensorBuf[3];
	mleft_des_speed = sensorBuf[4];
	mright_des_speed = sensorBuf[5];
	mleft_dist = sensorBuf[6] + (sensorBuf[7]<<8);
	mright_dist = sensorBuf[8] + (sensorBuf[9]<<8);
	adcLSL = sensorBuf[10] + (sensorBuf[11]<<8);
	adcLSR = sensorBuf[12] + (sensorBuf[13]<<8);
	adcMotorCurrentLeft = sensorBuf[14] + (sensorBuf[15]<<8);
	adcMotorCurrentRight = sensorBuf[16] + (sensorBuf[17]<<8);
	adcBat = sensorBuf[18] + (sensorBuf[19]<<8);
	adc0 = sensorBuf[20] + (sensorBuf[21]<<8);
	adc1 = sensorBuf[22] + (sensorBuf[23]<<8);
}

/*****************************************************************************/
/*****************************************************************************/
// INT0 Check:

#define INT0_STATUS_CHECK 0
uint8_t block = false;

/** 
 * This function has to be called VERY frequently out of the main loop.
 * Bigger delays result in slower reaction to Interrupt requests of the 
 * Slave. 
 * This function initiates a request of the first 3 Registers of the I2C Bus
 * Slave Controller - these Bytes contain status bits, which tell us what
 * caused the Interrupt request. 
 * They are checked in the requested data ready handler which should call
 * the function checkRP6Status below. 
 */
void task_checkINT0(void)
{
	if(!block && (PIND & EINT1)) 
	{
		block = true; // Block further requests and wait until 
					  // this request has been processed.
		I2CTWI_requestRegisterFromDevice(I2C_RP6_BASE_ADR, INT0_STATUS_CHECK, 0, 3);
	}
}

/*****************************************************************************/

// Bitmasks for comparision: 
#define MASK_ACS 	0b01100000 // the ACS Status bits... 
#define MASK_BUMPER 0b00000110 // Bumper bits
#define MASK_RC5    0b00001000 // RC5 Reception bit
#define MASK_BATLOW 0b00000001 // Low Voltage bit

uint8_t messageBuf[16]; 

/**
 * Call this function from the requestedDataReady Handler for the I2C Bus. 
 * It will then check if the request was a INT0 Request from this library and
 * then it will process the event and call event handlers is neccessary. 
 * It returns true if it was a INT0 request and false otherwise.
 * This way you can check if it was a request that you initiated or a 
 * request initiated in the library...
 */
uint8_t checkRP6Status(uint8_t dataRequestID)
{
	if(dataRequestID == INT0_STATUS_CHECK) 
	{            
                         
		// get received data: 
		I2CTWI_getReceivedData(messageBuf, 3);
		
		// We want to check if the ACS status bits have changed, so we XOR 
		// ('^' is eXclusive OR operator) them with old 
		// value for comparison and later mask them in the if conditions below...
		uint8_t compare = messageBuf[0] ^ interrupt_status.byte;
		interrupt_status.byte = messageBuf[0]; // Update local register

		// ------------------------------------
		// First, we check if the ACS status bits have changed, if not
		// we can just ignore the change as it was something else
		// and we only check the ACS bits in this example...
		if(compare & MASK_ACS) 
		{
			obstacle_left = interrupt_status.obstacleLeft;
			obstacle_right = interrupt_status.obstacleRight;
			ACS_stateChangedHandler();
		}
		
		// ------------------------------------
		// Check if Bumpers status has changed:
		if(compare & MASK_BUMPER) 
		{
			bumper_left = interrupt_status.bumperLeft;
			bumper_right = interrupt_status.bumperRight;
			BUMPERS_stateChangedHandler();
		}

		// ------------------------------------
		// Check if there was a RC5 Reception:
		uint8_t readBuf[2];
		if(compare & MASK_RC5) 
		{
			I2CTWI_transmitByte(I2C_RP6_BASE_ADR,I2C_REG_RC5_ADR);
			I2CTWI_readBytes(I2C_RP6_BASE_ADR, readBuf, 2);
			RC5data_t tmp;
			tmp.data = (readBuf[0] << 6) | (readBuf[1]);
			IRCOMM_RC5dataReadyHandler(tmp);
		}
		
		// ------------------------------------
		// Check for low Battery event:
		if(compare & MASK_BATLOW) 
		{
			I2CTWI_transmitByte(I2C_RP6_BASE_ADR,I2C_REG_ADC_UBAT_L);
			I2CTWI_readBytes(I2C_RP6_BASE_ADR, readBuf, 2);
			adcBat = (readBuf[1] << 8) | (readBuf[0]);
			BATTERY_lowVoltageHandler(interrupt_status.batLow);
		}
		
		// ------------------------------------
		// Check for WDT request:
		status.byte = messageBuf[1];
		if(status.wdtRequest)
			WDT_requestHandler();
		
		// ------------------------------------
		// Update drive Status:
		drive_status.byte = messageBuf[2];
		if(interrupt_status.driveSystemChange)
		{
			MOTIONCONTROL_stateChangedHandler();
		}

		block = false; // clear block flag! 
		return true; // It was an INT0 request --> return true;
	}
	return false; // It was not an INT0 request --> return false
}



/*****************************************************************************/
// New Movement functions:
/*****************************************************************************/

/**
 * This function is used in the movement functions to wait until the command
 * has been transmitted and is active. It takes some time until the 
 * movement register gets updated via the I2C Bus and the function behaves like 
 * with the RP6Lib - especially the drive_status.movementComplete flag!
 */
void waitForTransmitComplete(void)
{
	while(I2CTWI_isBusy() || TWI_operation != I2CTWI_NO_OPERATION) task_I2CTWI();
	mSleep(6);
	task_checkINT0();
	while(I2CTWI_isBusy() || TWI_operation != I2CTWI_NO_OPERATION) task_I2CTWI();
}

/**
 * Move at speed function - just the same as with RP6Lib!
 */
void moveAtSpeed(uint8_t desired_speed_left, uint8_t desired_speed_right)
{
	I2CTWI_transmit4Bytes(I2C_RP6_BASE_ADR, 0, CMD_MOVE_AT_SPEED, desired_speed_left, desired_speed_right );
	while(I2CTWI_isBusy() || TWI_operation != I2CTWI_NO_OPERATION) task_I2CTWI();
}

/**
 * Change direction function - just the same as with RP6Lib!
 */
void changeDirection(uint8_t dir)
{
	I2CTWI_transmit3Bytes(I2C_RP6_BASE_ADR, 0, CMD_CHANGE_DIR, dir);
	while(I2CTWI_isBusy() || TWI_operation != I2CTWI_NO_OPERATION) task_I2CTWI();
}

/**
 * Stop function - just the same as with RP6Lib!
 */
void stop(void)
{
	I2CTWI_transmit2Bytes(I2C_RP6_BASE_ADR, 0, CMD_STOP);
	waitForTransmitComplete();
}


uint8_t transmit_buffer[10]; // temporary transmit buffer

/**
 * Rotate function - just the same as with RP6Lib!
 */
void rotate(uint8_t desired_speed, uint8_t dir, uint16_t angle, uint8_t blocking)
{
	transmit_buffer[0] = 0;
	transmit_buffer[1] = CMD_ROTATE;
	transmit_buffer[2] = desired_speed;
	transmit_buffer[3] = dir;
	transmit_buffer[4] = ((angle>>8) & 0xFF);
	transmit_buffer[5] = (angle & 0xFF);
	I2CTWI_transmitBytes(I2C_RP6_BASE_ADR, transmit_buffer, 6 );
	// We need to wait until the I2C Bus transfer is complete and then 
	// check the movement status...
	waitForTransmitComplete();
	while(blocking && !drive_status.movementComplete) 
	{
		task_checkINT0();
		task_I2CTWI();
	}
}

/**
 * Move function - just the same as with RP6Lib!
 */
void move(uint8_t desired_speed, uint8_t dir, uint16_t distance, uint8_t blocking)
{
	transmit_buffer[0] = 0;
	transmit_buffer[1] = CMD_MOVE;
	transmit_buffer[2] = desired_speed;
	transmit_buffer[3] = dir;
	transmit_buffer[4] = ((distance>>8) & 0xFF);
	transmit_buffer[5] = (distance & 0xFF);
	I2CTWI_transmitBytes(I2C_RP6_BASE_ADR, transmit_buffer, 6 );
	waitForTransmitComplete();
	while(blocking && !drive_status.movementComplete) 
	{
		task_checkINT0();
		task_I2CTWI();
	}
}
