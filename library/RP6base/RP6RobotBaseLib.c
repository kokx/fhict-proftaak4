/* ****************************************************************************
 *                           _______________________
 *                           \| RP6  ROBOT SYSTEM |/
 *                            \_-_-_-_-_-_-_-_-_-_/         >>> BASE CONTROLLER
 * ----------------------------------------------------------------------------
 * ------------------- [c]2006 / 2007 - AREXX ENGINEERING ---------------------
 * -------------------------- http://www.arexx.com/ ---------------------------
 * ****************************************************************************
 * File: RP6RobotBaseLib.c
 * Version: 1.4
 * Target: RP6 Base - ATMEGA32 @8.00MHz
 * Author(s): Dominik S. Herwald
 * ****************************************************************************
 * Description:
 *
 * This is the RP6 Robot Base Library - it contains the following functions:
 * - Processor initialisation
 * - LED Control
 * - Bumpers
 * - A/D Convertor (Motor Current, Light Sensors, Battery Voltage...)
 * - Encoder reading and speed measurement 
 * - Motor Control (Automatic speed control and PWM + Direction control
 *   + failsafe functions (overcurrent and defect motor/encoder detection))
 * - ACS (Anti Collision System) 
 * - IRCOMM (IR-Communication System, RC5 transmission and reception)
 * - Timing functions (Delays, Stopwatches etc.)
 *
 * PLEASE ALSO READ THE RP6MANUAL! THERE YOU WILL FIND EXAMPLE
 * CODE AND SOME FURTHER EXPLANATIONS!
 *
 * In other parts of this library (RP6uart.c, RP6I2CSlaceTWI.c and 
 * RP6I2CMasterTWI.c)
 * you can find UART communication and I콯 Bus communication routines.
 *
 * -----
 * Hint: You should better leave all this as it is if you just started with
 * C programming, but it is a very good idea to read the comments and review 
 * the code, it will help you to understand C programming for AVR better!
 * -----
 *
 * For the experienced users: 
 * This code works OK, but it is not optimal! There is a lot potential for 
 * tuning! 
 * Well, this leaves some tasks for you and this is what makes most 
 * fun: To improve the excisting!  
 *
 * Of course you are free to add new functions and improvements to this
 * library and make them available to the public on the Internet e.g. on
 * our Forum!
 * Please use the changelog at the end of this file to document your
 * changes! And add your name to any new function or modification you added! 
 * E.g. a "modified by <name> at <date>" is always a good idea to show 
 * other users where and WHAT you changed in the source code!
 *
 * It is a good idea to make your own includeable libraries instead of
 * changing this library - of course only if this is possible.
 *
 * Or create your own complete library with all specific functions you need.
 * This code is GPL'd - s. license at the end of this file!
 *
 * ****************************************************************************
 * CHANGELOG AND LICENSING INFORMATION CAN BE FOUND AT THE END OF THIS FILE!
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h"

/*****************************************************************************/

/*****************************************************************************/
// Status LEDs:

// -------------------------------
// Power on warning
// 
#ifdef POWER_ON_WARNING
	uint8_t leds_on;

	/**
	 * Enables the "led-blink" warning after some time with
	 * no active LEDs. This is to ensure that you don't forget
	 * to turn of the Robot if your program does not use
	 * any LEDs for a long time! 
	 */
	void enablePowerOnWarning(void) 
	{ 
		if(leds_on > 3)
			leds_on = 0; 
	}
	
	/**
	 * This disables the power on warning. 
	 * also see RP6Config.h for #define POWER_ON_WARNING
	 */
	void disablePowerOnWarning(void) 
	{ 
		leds_on = 4; 
	}
#endif

/**
 * Update status LEDs with current value from shadow register.
 *
 * Additional info:
 * This function ensures that the LED pins are not driven low to allow
 * other circuitry to be connected to the I/O pads on the mainboard! 
 * As long as external circuits only connect the I/O pads to VCC and not to 
 * GND, everything should work fine, but always connect a >= 470 Ohm 
 * series resistor to limit maximum current!
 *
 *
 * Example:
 *
 *			statusLEDs.byte=0b101001;
 *			updateStatusLEDs();
 *			// this clears all LEDs and sets the LEDs STATUS1,
 *			// STATUS6 and STATUS4!
 *
 *			// Other possibility:
 *			statusLEDs.LED2=true;
 *			updateStatusLEDs();
 *			// This sets LED2 and does not affect any other LED!
 */
void updateStatusLEDs(void)
{
	DDRB &= ~0x83;
	PORTB &= ~0x83;
	if(statusLEDs.LED4){ DDRB |= SL4; PORTB |= SL4; }
	if(statusLEDs.LED5){ DDRB |= SL5; PORTB |= SL5; }
	if(statusLEDs.LED6){ DDRB |= SL6; PORTB |= SL6; }
	DDRC &= ~0x70;
	PORTC &= ~0x70;
	DDRC |= ((statusLEDs.byte << 4) & 0x70);
	PORTC |= ((statusLEDs.byte << 4) & 0x70);
	#ifdef POWER_ON_WARNING
		leds_on = (leds_on ? leds_on : (statusLEDs.byte && 1));
	#endif
}

/**
 * Set status LEDs - this is very handy if you want to set all LEDs.
 *
 * Example:
 *
 *			setLEDs(0b101001);
 *			// this clears all LEDs and sets the LEDs STATUS1,
 *			// STATUS6 and STATUS4!
 */
void setLEDs(uint8_t leds)
{
	statusLEDs.byte = leds;
	updateStatusLEDs();
}


/*****************************************************************************/
// Bumpers:

/**
 * Returns true if the the left Bumper is hit. 
 * This function turns off the LED connected to the port, reads the Bumper 
 * value and restores previous LED state afterwards!
 *
 * Example:
 *
 *		if(getBumperLeft())
 *			// do something
 */
uint8_t getBumperLeft(void)
{ 
	PORTB &= ~SL6;
	DDRB &= ~SL6; 
	nop();
	uint8_t tmp = PINB & SL6;
	if(statusLEDs.LED6) { 
		DDRB |= SL6; 
		PORTB |= SL6; 
	}
	return tmp;
}

/**
 * Returns true if the the right Bumper is hit. 
 * This function turns off the LED connected to the port, reads the Bumper 
 * value and restores previous LED state afterwards!
 *
 * Example:
 *
 *		if(getBumperRight())
 *			// do something
 */
uint8_t getBumperRight(void)
{
	PORTC &= ~SL3;
	DDRC &= ~SL3; 
	nop();
	uint8_t tmp = PINC & SL3;
	if(statusLEDs.LED3) { 
		DDRC |= SL3; 
		PORTC |= SL3; 
	}
	return tmp;
}

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
// -------------------------------

volatile uint8_t bumper_timer;
uint8_t bumper_left;
uint8_t bumper_right;

/**
 * If you call this frequently out of the mainloop (or use task_RP6System which
 * calls this routine for you), the global bumper_left and bumper_right
 * variables are updated automatically every 50ms and can be used everywhere
 * in your program. It can also call an event handler routine, that you
 * need to register with BUMPERS_setStateChangedHandler before.
 */
void task_Bumpers(void)
{
	if(bumper_timer > 50) { // 50ms
		uint8_t left = getBumperLeft();
		uint8_t right = getBumperRight();
		if(bumper_left != left || bumper_right != right) {
			bumper_left = left;
			bumper_right = right;
			BUMPERS_stateChangedHandler();
		}
		bumper_timer = 0;
	}
}

/*****************************************************************************/
// ADC:

/**
 * Read ADC channel (10 bit -> result is an integer from 0 to 1023).
 * The channels (ADC_BAT etc.) are defined in the RP6RobotBase.h file!
 *
 * This is a blocking function, which means it waits until the conversion
 * is complete. There is a more complicated alternative that frequently 
 * checks all channels (s. below).
 *
 * This function returns 0 if the ADC is buisy! This has been done to
 * prevents problems when the automatical function is used.
 * You should usually NOT use this function if you use the automatic one!
 *
 * Example:
 *
 *			uint16_t uBat = readADC(ADC_BAT);
 *			if(uBat < 600)
 *				writeString("WARNING: BAT IS LOW!\n");
 *
 */
uint16_t readADC(uint8_t channel)
{
	if((ADCSRA & (1<<ADSC))) return 0; // check if ADC is buisy...
	ADMUX = (1<<REFS0) | (0<<REFS1) | (channel<<MUX0);
	ADCSRA = (0<<ADIE) | (1<<ADSC) | (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADIF);
	while ((ADCSRA & (1<<ADSC))); 
	ADCSRA |= (1<<ADIF);
	return ADC;
}

/**
 * This function starts an ADC conversion - it does not return the
 * read value! You need to poll if the conversion is complete somewhere
 * else and then read it from the ADC result register.
 * (s. task_ADC function below)
 */
void startADC(uint8_t channel)
{
	ADMUX = (1<<REFS0) | (0<<REFS1) | (channel<<MUX0);
	ADCSRA = (0<<ADIE) | (1<<ADSC) | (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADIF);
}

// -----------------------

uint16_t adcBat;
uint16_t adcMotorCurrentLeft;
uint16_t adcMotorCurrentRight;
uint16_t adcLSL;
uint16_t adcLSR;
uint16_t adc0;
uint16_t adc1;

/**
 * This functions checks all ADC channels sequentially in the Background!
 * It can save a lot of time, if the ADC channels are checked like this, because
 * each A/D conversion takes some time. With this function you don't need to
 * wait until the A/D conversion is finished and you can do other things in the 
 * meanwhile.
 * If you use this function (this is also the case if you use task_RP6System 
 * because it calls this function), you can NOT use readADC anymore!
 *
 * Instead you can use the seven global variables you see above to
 * get the ADC values!
 */
void task_ADC(void)
{
	static uint8_t current_adc_channel = 0;
	if(!(ADCSRA & (1<<ADSC))) {
	//	ADCSRA |= (1<<ADIF);
		switch(current_adc_channel) {
			case 0: adcBat = ADC; startADC(ADC_MCURRENT_L); break;
			case 1: adcMotorCurrentLeft = ADC; startADC(ADC_MCURRENT_R); break;
			case 2: adcMotorCurrentRight = ADC; startADC(ADC_LS_L); break;
			case 3: adcLSL = ADC; startADC(ADC_LS_R); break;
			case 4: adcLSR = ADC; startADC(ADC_ADC0); break;
			case 5: adc0 = ADC; startADC(ADC_ADC1); break;
			case 6: adc1 = ADC; startADC(ADC_BAT); break;
		}
		if(current_adc_channel == 6)
			current_adc_channel = 0;
		else
			current_adc_channel++;
	}
}

/*****************************************************************************/
// Encoders

// Timing variable used for speed calculation:
volatile uint8_t speed_timer;

// Speed measurement variables
volatile uint16_t mleft_counter;
volatile uint16_t mright_counter;
volatile uint16_t mleft_speed;
volatile uint16_t mright_speed;

// Distance
volatile uint16_t mleft_dist;
volatile uint16_t mright_dist;

// This is only used for the selftest program.
// You don't need this for your own programs!
#ifdef DEBUG_MEASURE_DUTY_CYCLE
	volatile uint16_t cycle_h_l;
	volatile uint16_t cycle_l_l;
	volatile uint16_t cycle_h_r;
	volatile uint16_t cycle_l_r;
	
	volatile uint8_t cycle_h_l_tmp;
	volatile uint8_t cycle_l_l_tmp;
	volatile uint8_t cycle_h_r_tmp;
	volatile uint8_t cycle_l_r_tmp;
#endif

/**
 * External Interrupt 0 ISR
 * (ENCL)
 *
 */
ISR (INT0_vect)
{
	mleft_dist++;
	mleft_counter++;	
	
	// Only used for selftest program:
	#ifdef DEBUG_MEASURE_DUTY_CYCLE
		if(isEncoderLeft()) {
			cycle_l_l = cycle_l_l_tmp;
			cycle_l_l_tmp = 0;
		}
		else {
			cycle_h_l = cycle_h_l_tmp;
			cycle_h_l_tmp = 0;
		}
	#endif
}

/**
 * External Interrupt 1 ISR
 * (ENCR)
 *
 */
ISR (INT1_vect)
{
	mright_dist++;
	mright_counter++;
	
	// Only used for selftest program:
	#ifdef DEBUG_MEASURE_DUTY_CYCLE
		if(isEncoderLeft()) {
			cycle_l_r += cycle_l_r_tmp;
			cycle_l_r >>=1;
			cycle_l_r_tmp = 0;
		}
		else {
			cycle_h_r += cycle_h_r_tmp;
			cycle_h_r >>=1;
			cycle_h_r_tmp = 0;
		}
	#endif
}


/*****************************************************************************/
// Motor speed control

#define MC_LEFT_IMAX 420
#define MC_LEFT_IMIN -420
#define MC_RIGHT_IMAX 420
#define MC_RIGHT_IMIN -420

int16_t right_i; 
int16_t left_i; 
int16_t mleft_power;
int16_t mright_power;
uint8_t mleft_ptmp;
uint8_t mright_ptmp;

uint16_t mleft_des_speed;
uint16_t mright_des_speed;
uint16_t mleft_des_speed_tmp;
uint16_t mright_des_speed_tmp;

uint8_t mleft_des_dir = FWD;
uint8_t mright_des_dir = FWD;
uint8_t mleft_dir = FWD;
uint8_t mright_dir = FWD;

uint8_t overcurrent_timeout;
uint8_t overcurrent_timer;
uint8_t overcurrent_errors;
uint8_t overcurrent_error_clear;

uint8_t motion_status_tmp;

uint16_t distanceToMove_L;
uint16_t distanceToMove_R;
uint16_t preDecelerate_L;
uint16_t preDecelerate_R;
uint16_t preStop_L;
uint16_t preStop_R;

volatile uint8_t motor_control;


// -------------------------------
// MotionControl state changed handler:

void MOTIONCONTROL_stateChanged_DUMMY(void){}
static void (*MOTIONCONTROL_stateChangedHandler)(void) = MOTIONCONTROL_stateChanged_DUMMY;
/**
 * Use this function to set the Motion Control state change handler. 
 * 
 */
void MOTIONCONTROL_setStateChangedHandler(void (*motionControlHandler)(void)) 
{
	MOTIONCONTROL_stateChangedHandler = motionControlHandler;
}
// -------------------------------


// Outcomment this:
#define ENABLE_OC_ERROR_MESSAGE
// to disable the error messages (s. below) in order to save some program space!
// The motionControl function will still check for any errors and stop all operations
// if neccessary.
// This option just disables the error message for overcurrent or 
// defect encoder/motor events.

// Error type:
#define ENCODER_MALFUNCTION_LEFT 1
#define ENCODER_MALFUNCTION_RIGHT 2
#define OVERCURRENT 3

/**
 * If there are any errors like overcurrent or encoder malfunction, this function is 
 * called from task_motionControl (s. below) and outputs an error message and then stops
 * all operations to save the robot from damages that may occur if it continues
 * to drive around. 
 */
void emergencyShutdown(uint8_t why)
{
	cli();
	IRCOMM_OFF();
	setACSPwrOff();
	mleft_power = 0;
	mright_power = 0;
	left_i = 0;
	right_i = 0;
	mleft_ptmp = 0;
	mright_ptmp = 0;
	OCR1AL = 0;
	OCR1BL = 0;
	TCCR1A = 0;
	powerOFF();
#ifdef ENABLE_OC_ERROR_MESSAGE
	writeString_P("\n\n##### EMERGENCY SHUTDOWN #####\n");
	writeString_P("##### ALL OPERATIONS STOPPED TO PREVENT ANY DAMAGE! #####\n");
	if(why == ENCODER_MALFUNCTION_LEFT || why == ENCODER_MALFUNCTION_RIGHT) {
		
		writeString_P("\n### ENCODER (OR MOTOR) MALFUNCTION! ###\n");
		writeString_P("Affected channel:"); 	
		if(why == ENCODER_MALFUNCTION_LEFT)
			writeString_P("LEFT");
		else
			writeString_P("RIGHT");
		writeString_P("!\n\n(s. task_motorControl() function in RP6Lib!)\n");
		writeString_P("You need to check Encoder/Motor assembly (or your software).\n\n");
	}
	else if(why == OVERCURRENT)
	{
		writeString_P("\n\n### MOTOR OVERCURRENT ###\n");
		writeString_P("\n\n(s. task_motorControl() function in RP6Lib!)\n");
		writeString_P("You need to check Motor assembly (or your software).\n\n");
	}
	writeString_P("The Robot needs to be resetted now.\n\n");
#endif
	while(true) // Rest In Peace
	{
		setLEDs(0b110110);
		uint8_t dly;
		for(dly = 10; dly; dly--)
			delayCycles(32768);
		setLEDs(0b000000);
		for(dly = 10; dly; dly--)
			delayCycles(65535);
	}
}

/**
 * Automatic motor speed regulation, direction control, move specific distances and
 * overcurrent+motor+encoder malfunction detection.
 * You must call this function frequently out of the main loop if you
 * want to use the motors! (this call is included in the task_RP6System function!)
 * 
 * This is the most important motor control function!
 *
 * Integral regulation only - works OK but could be improved by 
 * implementing PI or PID regulation.
 * 
 * The speed increase is limited by Soft-PWM adjustment (can also be used to make the
 * acceleration of the motors a lot slower just by software) in the Timer0 interrupt.
 * This intends to increase motor and gears life - along with slow turning direction
 * change! Fast changes at high speeds can wear out the gears over time. 
 *
 * You need to consider this Soft-PWM when changing/tuning this function!
 *
 */
void task_motionControl(void)
{
	// Automatic motor overcurrent shutdown:
	if(overcurrent_timer >= 50) { // every 5ms
		overcurrent_timer = 0;
		if(!overcurrent_timeout) {
			if((adcMotorCurrentLeft > 770) || (adcMotorCurrentRight > 770)) {
				overcurrent_errors++;
				overcurrent_timeout = 10; 
				mleft_power = 0;
				mright_power = 0;				
				left_i = 0;
				right_i = 0;
				motion_status.overcurrent = true;
				return;
			}
			else
				motion_status.overcurrent = false;
			
			// Emergency shutdown if there are too many (default: 3) overcurrent
			// events within ~20 seconds (100 * 200ms).
			if(overcurrent_error_clear > 100) {
				overcurrent_errors = 0;
				overcurrent_error_clear = 0;
			}
			else if(overcurrent_errors > 2)
				emergencyShutdown(OVERCURRENT);
		}
		
		// Detect if one of the encoders or motors does not work properly and stop 
		// all operations immediately if this is the case! 
		if((adcMotorCurrentLeft < 150) && (mleft_speed == 0) 
		  && (mleft_des_speed != 0) &&  (mleft_ptmp > 150))
			emergencyShutdown(ENCODER_MALFUNCTION_LEFT);
		if((adcMotorCurrentRight < 150) && (mright_speed == 0) 
		  && (mright_des_speed != 0) && (mright_ptmp > 150))
			emergencyShutdown(ENCODER_MALFUNCTION_RIGHT);
	}
	
	// Motor Control
	if(motor_control) { // Everytime after the speed has been measured. (default: 200ms)
		if(!overcurrent_timeout) { // No overcurrent timeout? (default is to wait 2 seconds before new try)
			if(overcurrent_errors) // Overcurrent errors?
				overcurrent_error_clear++; // Yes, Timeout to clear all error events.
			else
				overcurrent_error_clear=0; // No, we set the timeout to zero.
				
			// Move Distance left:
			if(motion_status.move_R) {
				if(mleft_dist >= preStop_R) { // Stop a bit before the desired distance for ..
					mleft_des_speed = 0;      // ... better accurancy.
					left_i = 0;
					mleft_power = 0;		
					motion_status.move_R = false;
				}
				else if(mleft_dist >= preDecelerate_R) { // Start to decelerate?
					mleft_des_speed /= 2;
					if(mleft_des_speed < 22) mleft_des_speed = 22;
				}	
			}
			
			// Move Distance right:
			if(motion_status.move_L) {
				if(mright_dist >= preStop_L) { // Stop a bit before the desired distance for ..
					mright_des_speed = 0;      // ... better accurancy.
					right_i = 0;
					mright_power = 0;
					motion_status.move_L = false;
				}
				else if(mright_dist >= preDecelerate_L) { // Start to decelerate?
					mright_des_speed /= 2;
					if(mright_des_speed < 22) mright_des_speed = 22;
				}	
			}
			
			// Change direction -- slowdown and stop before changing direction  
			// to improve gears and motors lifetime:
			if(mleft_des_dir != mleft_dir || mright_des_dir != mright_dir) {
				if(mleft_des_speed || mright_des_speed) {
					mleft_des_speed_tmp = mleft_des_speed; // store current speed
					mright_des_speed_tmp = mright_des_speed; 
					mleft_des_speed = 0;			
					mright_des_speed = 0;
					left_i /= 2;
					right_i /= 2;
				}
				if(mright_speed <= 25 && mleft_speed <= 25 
				  && (!mleft_des_speed) && (!mright_des_speed)) {
					mright_power=0; // Soft PWM adjust to 0
					mleft_power=0;
				}
				if(!TCCR1A) // Is the PWM module turned off?
				{			// Yes, change direction and restore old speed:
					setMotorDir(mleft_des_dir,mright_des_dir);
					mleft_des_speed = mleft_des_speed_tmp;
					mright_des_speed = mright_des_speed_tmp;
					left_i = 0;
					right_i = 0;
				}
			}

			// Left motor speed control:
			int16_t error_left = mleft_des_speed - mleft_speed;
			left_i = left_i + error_left;
			if(left_i > MC_LEFT_IMAX) left_i = MC_LEFT_IMAX;
			if(left_i < MC_LEFT_IMIN) left_i = MC_LEFT_IMIN;
			if(mleft_speed == 0 && mleft_des_speed == 0)
				left_i = 0;
			mleft_power = left_i / 2; 
			if(mleft_power > 210) mleft_power = 210;
			if(mleft_power < 0) mleft_power = 0;
			
			// Right motor speed control:
			int16_t error_right = mright_des_speed - mright_speed;
			right_i = right_i + error_right;
			if(right_i > MC_RIGHT_IMAX) right_i = MC_RIGHT_IMAX;
			if(right_i < MC_RIGHT_IMIN) right_i = MC_RIGHT_IMIN;
			if(mright_speed == 0 && mright_des_speed == 0)
				right_i = 0;
			mright_power = right_i / 2;
			if(mright_power > 210) mright_power = 210;
			if(mright_power < 0) mright_power = 0;
		}
		else
			overcurrent_timeout--;
		motor_control = false;
	}
	
	// Call event handlers if necessary:
	if(motion_status_tmp != motion_status.byte)
	{
		motion_status_tmp = motion_status.byte;
		MOTIONCONTROL_stateChangedHandler();
	}
}

/**
 * This function sets the desired speed value. 
 * The rest is done automatically. The speed is regulated to this speed value 
 * independent of Battery voltage, surface, weight of the robot and other things. 
 *
 * You need to call task_motorSpeedControl();  frequently out of the main loop!
 * otherwise this function will not work!
 * Or use task_RP6System which includes this call. 
 *
 * The function limits maximum speed to 200! This has been done because the maximum
 * possible speed gets lower over time due to battery discharging (--> voltage drop).
 * And the gears and motors will live longer if you don't stress them that much.
 *
 * Also 200 leaves a bit room to the maximum possible PWM value when you 
 * put additional load onto the Robot or drive up a ramp etc.  
 *
 */
void moveAtSpeed(uint8_t desired_speed_left, uint8_t desired_speed_right)
{
	if(desired_speed_left > 200) desired_speed_left = 200; 
	if(desired_speed_right > 200) desired_speed_right = 200;
	mleft_des_speed = desired_speed_left;
	mright_des_speed = desired_speed_right;
}

uint8_t drive_dir = FWD;

/**
 * This functions sets the desired turning direction of the two motors. 
 * The Robot first slows down, stops, changes direction and then it accelerates again
 * to the previours speed (if the robot was driving... ).  
 * This is done to increase motors and gears lifetime and to avoid hard cut changes.
 *
 */
void changeDirection(uint8_t dir)
{
	drive_dir = dir;
	mleft_des_dir = (dir == BWD || dir == LEFT);
	mright_des_dir = (dir == BWD || dir == RIGHT);
}

/**
 * You can use this function to check if there is any movement going on or if
 * every operation like moving a specific distance or rotating has been finished. 
 */
uint8_t isMovementComplete(void)
{
	return !(motion_status.move_L || motion_status.move_R);
}

/**
 * If there are any operations like moving a specific distance or rotating or
 * any motion at all, you can stop the robot with this function. 
 * This can be used for example if the Bumpers detected and obstacle during
 * movement...
 */
void stop(void)
{
	mleft_des_speed = 0;
	mright_des_speed = 0;
	left_i = 0;
	right_i = 0;
	motion_status.move_L = false;
	motion_status.move_R = false;
	motion_status_tmp = motion_status.byte;
	MOTIONCONTROL_stateChangedHandler();
}


/**
 * The robot can use the encoders to measure the distance it has driven. 
 * This function allows you to tell the robot to drive a specific distance 
 * at a given speed and then stop. 
 * Of course this only works if you calibrated the Encoders before! 
 * Also read the RP6 Manual for additional details and have a look at the
 * movement example programs where you also find a detail description of
 * the parameters along with usage examples! 
 *
 * The function sets some parameters that make the robot decelerate a bit
 * before reaching the target distance. This makes it more accurate.
 * If you need faster reaction rather than precision then you should implement 
 * your own routine and set the distance a bit lower... 
 *
 */
void move(uint8_t desired_speed, uint8_t dir, uint16_t distance, uint8_t blocking)
{
	motion_status.move_L = true;
	motion_status.move_R = true;
	preDecelerate_L = 0;
	preDecelerate_R = 0;
	if(desired_speed > 22) {
		preDecelerate_L = distance - (20+(desired_speed*2));
		preDecelerate_R = preDecelerate_L;
	}
	preStop_L = distance - 2;
	preStop_R = preStop_L;
	if(distance < 40) {
		distance = 40; 
		preStop_L = 20;
		preStop_R = preStop_L;
		preDecelerate_L = 10;
		preDecelerate_R = preDecelerate_L;
	}
	if(distance < 400 && desired_speed > 40) {
		desired_speed = 40; 
		preDecelerate_L = distance - (distance/4);
		preDecelerate_R = preDecelerate_L;
	}
    mleft_dist = 0; 
	mright_dist = 0;
	moveAtSpeed(desired_speed,desired_speed);
	changeDirection(dir);
	
	distanceToMove_L = distance;
	distanceToMove_R = distance;

	motion_status_tmp = motion_status.byte;
	
	motion_status_tmp = motion_status.byte;
	MOTIONCONTROL_stateChangedHandler();
	
	if(blocking)
		while(!isMovementComplete())
			task_RP6System();
}

/**
 * You can let the Robot rotate specific angles with this function. 
 * ATTENTION: THIS IS NOT PRECISE!!! (+-10� after calibrating the 
 * Encoders depending on the floor surface)
 *
 * The main problem is tracks slippery - the encoders usually measure far bigger
 * distances than the Robot actually turns! 
 * This depends on the surface you rotate on and also on rotation speed! Thus you have 
 * to do experiments with this function or your own implementation on the surfaces 
 * you want to use the Robot on. The Robot will behave different for example
 * on a carpet compared to a wooden floor!  
 * It will even behave a bit different depending on the weight of the Robot!
 * 
 * This usually does not affect the precision when driving forwards! 
 * It only affects rotation! 
 *
 * This means that you can use external sensors for rotation in order to make it
 * more accurate. For example an electronic compass. 
 * 
 */
void rotate(uint8_t desired_speed, uint8_t dir, uint16_t angle, uint8_t blocking)
{
	motion_status.move_L = true;
	motion_status.move_R = true;
	uint16_t distance = (uint16_t) (((uint32_t)(ROTATION_FACTOR) * (uint16_t)angle)/100);
	preDecelerate_L = distance - 100;
	preDecelerate_R = distance - 100;
	preStop_L = distance;
	preStop_R = distance;
	if(distance < 40) {
		distance = 40; 
		preStop_L = 20;
		preStop_R = 20;
		preDecelerate_L = 10;
		preDecelerate_R = 10;
	}
	moveAtSpeed(desired_speed,desired_speed);
	changeDirection(dir);
	
    mleft_dist = 0; 
	mright_dist = 0;
	distanceToMove_L = distance;
	distanceToMove_R = distance;
	
	motion_status_tmp = motion_status.byte;
	MOTIONCONTROL_stateChangedHandler();
	if(blocking)
		while(!isMovementComplete())
			task_RP6System();
}


/*****************************************************************************/
/*****************************************************************************/

/**
 * Set Motor Power (PWM Value - proportional Speed - this is NOT the real
 * speed value, the real speed depends on weight of your RP6,
 * surface, battery voltage and various other things).
 * This function just sets PWM duty cycle and thus the mean voltage present 
 * at the motors. 
 * PWM is turned off when speed of both motors is set to 0.
 * Even this function does not set the PWM Value directly, the PWM value
 * is soft-ramped up and down in the Timer 0 Interrupt to make the changes a
 * bit slower (increased gears and Motor lifetime)! 
 * It is still quite fast, so keep in mind that you should not change the speed 
 * by big values in short time - this could damage the gears and the Motor if you 
 * do this too often. 
 * It will not instantly damage the motors/gears, but they will wear out much 
 * faster if you do not use soft speed adjustment.)
 *
 * IMPORTANT:
 * MAX PWM VALUE DEPENDS ON TIMER1 SETTINGS! (see comment in init function!)
 * Maximum value is 210 per default and NOT 255 as you might have thought!
 *
 * Again: 210 is the MAXIMUM Value! This function automatically limits the 
 * speed value to this limit.
 * You should not let the motors run at 210 all the time! This is just the
 * maximum value - better let them operate well below this maximum value!
 *
 * -------------------------------------------------------------
 * IT IS A BETTER IDEA NOT TO USE THIS FUNCTION AT ALL!
 * Use moveAtSpeed together with task_motionControl instead.
 * YOU CAN NOT USE setMotorPower AND setMotorDir WHEN YOU USE 
 * task_motionControl!  This will not work!
 * -------------------------------------------------------------
 *
 */
void setMotorPower(uint8_t left_power, uint8_t right_power)
{
	if(left_power > 210) left_power = 210;
	if(right_power > 210) right_power = 210;
	mright_power = right_power;
	mleft_power = left_power;
}

/**
 * Sets the rotation direction of both motors.
 *
 * DO NOT CHANGE THE DIRECTION OF THE MOTORS WHILE THEY
 * ARE RUNNING AT HIGH SPEEDS!!! 
 * It will not instantly damage the motors/gears, but they will  
 * wear out much faster if you do it at high speeds - better wait 
 * until speed has slowed down - and change direction AFTER this.
 *
 * -------------------------------------------------------------
 * IT IS A BETTER IDEA NOT TO USE THIS FUNCTION AT ALL!
 * Use moveAtSpeed together with task_motionControl instead.
 * YOU CAN NOT USE setMotorPower AND setMotorDir WHEN YOU USE 
 * task_motionControl!  This will not work!
 * -------------------------------------------------------------
 *
 * task_motionControl also ensures that the direction is changed
 * slowly and only after the motors have stopped!
 *
 * Example:
 *          // DO NOT perform these commands directly after each
 *			// other in your programs - this is just a LIST of possible
 *			// combinations:
 *			setMotorDir(FWD,FWD); // Move forwards
 *			setMotorDir(BWD,FWD); // Rotate right
 *			setMotorDir(FWD,BWD); // Rotate left
 *			setMotorDir(BWD,BWD); // Move backwards
 *
 */
void setMotorDir(uint8_t left_dir, uint8_t right_dir)
{
	mleft_dir = left_dir;
	mright_dir = right_dir;
	mleft_des_dir = left_dir;
	mright_des_dir = right_dir;
	if(left_dir)
		PORTC |= DIR_L;
	else
		PORTC &= ~DIR_L;
	if(right_dir)
		PORTC |= DIR_R;
	else
		PORTC &= ~DIR_R;
}


/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
// Anti Collision System (ACS) 
// and IR-Communication (IRCOMM)

// -------------------------------------------------
// ACS definitions, variables and bitfields:

#define ACS_STATE_IDLE 0
#define ACS_STATE_IRCOMM_DELAY 1
#define ACS_STATE_SEND_LEFT 2
#define ACS_STATE_WAIT_LEFT 3
#define ACS_STATE_SEND_RIGHT 5
#define ACS_STATE_WAIT_RIGHT 6

#define ACS_CHANNEL_LEFT 0
#define ACS_CHANNEL_RIGHT 1

uint8_t acs_state = ACS_STATE_IRCOMM_DELAY;
volatile uint16_t acs_timer; 
volatile uint8_t acs_pulse;
volatile uint8_t acs_event_counter;
uint8_t obstacle_left;
uint8_t obstacle_right;

union {
	uint8_t byte;
	struct {
		unsigned channel:1;
		unsigned pin:1;
		unsigned acs_int:1;
		unsigned rc5_data_received:1;
		volatile unsigned detect_rc5:1;
		unsigned acs_go:1;
		volatile unsigned ircomm_transmit:1;
		unsigned reserved:1;
	};
} sysStatACS;

// -------------------------------
// IRCOMM
volatile uint8_t ircomm_send;
volatile uint16_t ircomm_data;
uint16_t ircomm_data_tmp;

volatile RC5data_t IRCOMM_RC5_data_ok;
volatile RC5data_t IRCOMM_RC5_data; 

/**
 * This function initiates a RC5 transmission with the IRCOMM. 
 * You need to call the "task_ACS()" function (s. below) frequently
 * out of the main loop - otherwise it will not work! (this is required
 * because it needs to be synchronized with the ACS...)
 * The transmission itself is interrupt based and takes about 20ms.
 * Warning: The IRCOMM_sendRC5 function blocks ACS operation until
 * transmission is finished!
 * 
 * The address byte contains the device address and the togglebit. 
 * Address is 5 bit + 1 toggle bit, data is 6 bit.
 * You can use some of the address bits for data if you need 8 bit 
 * data!
 * 
 * Example: 
 * 	IRCOMM_sendRC5(10 | TOGGLEBIT, 40); 
 *  // This transmits 40 to the device with address 10.
 * 	// The togglebit is set.
 *
 * 	IRCOMM_sendRC5(10, 40); 
 *  // This is nearly the same but the Togglebit is NOT set.
 *
 *  IRCOMM_sendRC5(30, 60);
 *  // This is another transmission with device address 30 and 60 as data
 *  // with togglebit NOT set.
 *  
 */
void IRCOMM_sendRC5(uint8_t adr, uint8_t data)
{
	while(ircomm_send){TIMSK |= (1 << OCIE2);}
	// Here we create the RC5 data packet:
	ircomm_data_tmp = 0x3000 | (((uint16_t)(adr & 0x3F)) << 6) | (((uint16_t)(data & 0x3F)));
	sysStatACS.ircomm_transmit = true;
}

// -------------------------------

/**
 * Timer 2 Compare ISR
 * ACS & IRCOMM
 *
 * WARNING: DO NOT CHANGE THIS!
 * NEVER try to control the IRCOMM by your own routines if you do not
 * know what you are doing!
 *
 */
ISR (TIMER2_COMP_vect)
{
	static uint8_t ircomm_pulse;
	if(acs_state < 2) { // If ACS is not active, perform IRCOMM transmissions
		if(ircomm_pulse) { // Do we have IR pulses to send?
			if(ircomm_pulse < 60) { // Bi-Phase encoding...
				if(ircomm_data & 0x4000) // check current bit
					PORTD ^= (1<<PIND7); // Toggle IRCOMM port
				else
					PORTD &= ~(1<<PIND7); // deactivate IRCOMM port
			}
			else if(ircomm_data & 0x4000) // The same as above, but the other way round:
				PORTD &= ~(1<<PIND7); // deactivate IRCOMM port
			else
				PORTD ^= (1<<PIND7); // Toggle IRCOMM port
			ircomm_pulse--;
		}
		else if(ircomm_send) { // Do we still have data?
			PORTD &= ~(1<<PIND7);
			ircomm_data <<= 1; // Next Bit!
			ircomm_pulse = 120;
			ircomm_send--;
		}
		else 
			PORTD &= ~(1<<PIND7); // no more pulses - IR LEDs off!
	}
	else if(acs_pulse) { // Send ACS IR pulses?
		if(sysStatACS.channel == ACS_CHANNEL_LEFT) // which channel?
			PORTB ^= ACS_L; 
		else 			
			PORTC ^= ACS_R; 
		acs_pulse--;
	}
	else { // no more pulses - IR LEDs off!
		PORTB |= ACS_L;
		PORTC |= ACS_R;
	}
}

/**
 * External Interrupt 2 ISR (ACS)
 * Detects ACS Events.
 */
ISR (INT2_vect)
{
	if(acs_state == ACS_STATE_WAIT_LEFT || acs_state == ACS_STATE_WAIT_RIGHT)
		if(!sysStatACS.detect_rc5 && sysStatACS.acs_go && !(PINB & ACS))
			acs_event_counter++;
	sysStatACS.pin = (PINB & ACS);
}

// -------------------------------
// RC5 Data reception Handler:

void IRCOMM_RC5dataReady_DUMMY(RC5data_t rc5data){}
static void (*IRCOMM_RC5dataReadyHandler)(RC5data_t rc5data) = IRCOMM_RC5dataReady_DUMMY;
/**
 * Use this function to set the RC5 reception handler. 
 * 
 * Example:
 * 
 *  void receiveRC5Data(RC5data_t rc5data)
 *	{
 *		writeString_P("Toggle Bit:");
 *		writeChar(rc5data.toggle_bit + '0');
 *		writeString_P(" | Device Address:");
 *		writeInteger(rc5data.device, DEC);
 *		writeString_P(" | Key Code:");
 *		writeInteger(rc5data.key_code, DEC);
 *		writeChar('\n');
 *      // This is just an example and prints out the received data. 
 *		// You can do other things here like check for specific keycodes...
 *	}
 * 
 *  // [...]
 *  // in the main function or somewhere else:
 *  IRCOMM_setRC5DataReadyHandler(receiveRC5Data);
 *  // [...]
 *
 * With this setup, the function receiveRC5Data would be called everytime
 * the IRCOMM receives a RC5 Data packet.
 * 
 */
void IRCOMM_setRC5DataReadyHandler(void (*rc5Handler)(RC5data_t rc5data)) 
{
	IRCOMM_RC5dataReadyHandler = rc5Handler;
}

// -------------------------------


// -------------------------------
// ACS State changed handler:

void ACS_stateChanged_DUMMY(void){}
static void (*ACS_stateChangedHandler)(void) = ACS_stateChanged_DUMMY;
/**
 * Use this function to set the ACS state changed handler. 
 * The function you register will be called everytime when the ACS
 * has different status - e.g. if it suddenly detects and obstacle
 * OR if the obstacle moves out of the line of sight and ACS
 * reports "Way is free" again.
 */
void ACS_setStateChangedHandler(void (*acsHandler)(void)) 
{
	ACS_stateChangedHandler = acsHandler;
}


/**
 * This is the ACS task.
 * It should be called frequently out of the main loop.
 *
 * ACS is synchronized with RC5 transmission/reception and works also during 
 * RC5 data transmissions. If other _non_ RC5 code IR transmission are active, 
 * it will maybe NOT work during this time or it may detect invisible obstacles!
 * You need to call this function all the time if you want to transmit or receive
 * RC5 data with the IRCOMM!
 *
 * Example:
 *
 * 		// Main loop 
 *		while(true) 
 *		{
 *			task_ACS();
 *			//... other things ...
 *		}
 *
 */
 

void task_ACS(void)
{
 	static uint8_t acs_counter;
	static uint16_t acs_detect_timeout;
	if(acs_timer >= ACS_UPDATE_INTERVAL) { 
		if(!sysStatACS.detect_rc5) {    // Any RC5 reception detected?
			switch(acs_state) {       // No - perform IR Transmission and ACS tasks...
				case ACS_STATE_IDLE: // Disable Timer2 interrupt to save processing time:
					TIMSK &= ~(1 << OCIE2);
					IRCOMM_OFF();
					PORTB |= ACS_L;
					PORTC |= ACS_R;
				break;
				case ACS_STATE_IRCOMM_DELAY: // Check for IRCOMM transmit data:
					if(!ircomm_send) {  // Transmission finished?
						if(sysStatACS.ircomm_transmit) { // New transmission?
							ircomm_data = ircomm_data_tmp;
							ircomm_send = 14;
							sysStatACS.ircomm_transmit = false;
							TIMSK |= (1 << OCIE2);
						}
						else {
							TIMSK &= ~(1 << OCIE2);
							IRCOMM_OFF();
							PORTB |= ACS_L;
							PORTC |= ACS_R;
						}
						if(acs_counter++ >= ACS_IRCOMM_WAIT_TIME) // Delay 
							acs_state=ACS_STATE_SEND_LEFT;
					}
					else
						TIMSK |= (1 << OCIE2);
					if(sysStatACS.rc5_data_received) { // RC5 data received? 
						IRCOMM_RC5dataReadyHandler(IRCOMM_RC5_data_ok); // Call handler
						IRCOMM_RC5_data	= IRCOMM_RC5_data_ok;
						IRCOMM_RC5_data_ok.data = 0;
						sysStatACS.rc5_data_received = false;
					}
				break;
				case ACS_STATE_SEND_LEFT:  // Prepare left channel:
					TIMSK &= ~(1 << OCIE2);
					IRCOMM_OFF();
					PORTB |= ACS_L;
					PORTC |= ACS_R;
					acs_pulse = 0;
					acs_event_counter = 0;
					acs_detect_timeout = 0;
					sysStatACS.channel = ACS_CHANNEL_LEFT;
					sysStatACS.acs_go = true;
					acs_counter = 3; 
					acs_state = ACS_STATE_WAIT_LEFT;
				break;
				case ACS_STATE_WAIT_LEFT: // Wait for reception of IR pulses from Left channel
					if(!acs_pulse && acs_counter++ > 2) { 
						TIMSK |= (1 << OCIE2); 
						acs_pulse = ACS_SEND_PULSES_LEFT;  // Send pulses! 
						acs_counter = 0;
					}
					if(obstacle_left && acs_event_counter >= ACS_REC_PULSES_LEFT_THRESHOLD) {
						acs_event_counter = 0;
						obstacle_left = true;
						acs_state = ACS_STATE_SEND_RIGHT;
					}
					else if(acs_event_counter >= ACS_REC_PULSES_LEFT) { // receive min. ACS_REC_PULSES_LEFT pulses  
						acs_event_counter = 0;
						obstacle_left = true;
						acs_state = ACS_STATE_SEND_RIGHT;
					}
					else if(acs_detect_timeout++ >= ACS_TIMEOUT_LEFT) { // Timeout?
						obstacle_left = false;
						acs_state = ACS_STATE_SEND_RIGHT;
						TIMSK &= ~(1 << OCIE2);
						IRCOMM_OFF();
						PORTB |= ACS_L;
						PORTC |= ACS_R;
						acs_pulse = 0;
					}
				break;
				case ACS_STATE_SEND_RIGHT:   // Prepare right channel:
					TIMSK &= ~(1 << OCIE2);
					IRCOMM_OFF();
					PORTB |= ACS_L;
					PORTC |= ACS_R;
					acs_pulse = 0;
					acs_event_counter = 0;
					acs_detect_timeout = 0;
					sysStatACS.channel = ACS_CHANNEL_RIGHT;
					sysStatACS.acs_go = true;
					acs_counter = 3;
					acs_state = ACS_STATE_WAIT_RIGHT;
				break;
				case ACS_STATE_WAIT_RIGHT:  // Wait for reception of IR pulses
					if(!acs_pulse && acs_counter++ > 2) { 
						TIMSK |= (1 << OCIE2);
						acs_pulse = ACS_SEND_PULSES_RIGHT; 	// Send pulses!
						acs_counter = 0;
					}
					if(obstacle_right && acs_event_counter >= ACS_REC_PULSES_RIGHT_THRESHOLD) {
						acs_event_counter = 0;
						obstacle_right = true;
						acs_state = ACS_STATE_IRCOMM_DELAY;
					}
					else if(acs_event_counter >= ACS_REC_PULSES_RIGHT) { // receive min. ACS_REC_PULSES_RIGHT pulses
						acs_event_counter = 0;
						obstacle_right = true;
						acs_state = ACS_STATE_IRCOMM_DELAY;
					}
					else if(acs_detect_timeout++ >= ACS_TIMEOUT_RIGHT) { // Timeout?
						obstacle_right = false;
						acs_state = ACS_STATE_IRCOMM_DELAY;
						TIMSK &= ~(1 << OCIE2);
						IRCOMM_OFF();
						PORTB |= ACS_L;
						PORTC |= ACS_R;
						acs_pulse = 0;
					}
				break;
			}
		}
		else { // RC5 reception detected...
			acs_detect_timeout = 0;
			acs_counter = 0;
		}
		
		// Check for changes and call event handler if necessary:
		static uint8_t acs_l_tmp;
		static uint8_t acs_r_tmp;
		if(acs_l_tmp != obstacle_left || acs_r_tmp != obstacle_right) { // Did the ACS Status change?
			acs_l_tmp = obstacle_left;   // Yes, update and call event handler...
			acs_r_tmp = obstacle_right;
			ACS_stateChangedHandler();
		}
		acs_timer = 0;
	}
}

/**
 * Disables the ACS task.
 * ACS and IRCOMM Transmissions/Receptions will not work anymore.
 */
void disableACS(void)
{
	acs_state = ACS_STATE_IDLE;
	TIMSK &= ~(1 << OCIE2);
	IRCOMM_OFF();
	PORTB |= ACS_L;
	PORTC |= ACS_R;
	obstacle_right = false;
	obstacle_left = false;
}

/**
 * Enables the ACS task.
 */
void enableACS(void)
{
	TIMSK &= ~(1 << OCIE2);
	IRCOMM_OFF();
	PORTB |= ACS_L;
	PORTC |= ACS_R;
	obstacle_right = false;
	obstacle_left = false;
	acs_state = ACS_STATE_IRCOMM_DELAY;
}

/**
 * Turn ACS Power off.
 *
 * Example:
 *
 *			setACSPwrOff();
 *
 */
void setACSPwrOff(void)
{
	DDRD &= ~ACS_PWR;
	PORTD &= ~ACS_PWR;
	DDRB &= ~ACS_PWRH;
	PORTB &= ~ACS_PWRH;
	PORTB &= ~ACS_L;
	PORTC &= ~ACS_R;
}

/**
 * Set ACS Power low.
 *
 * Example:
 *
 *			setACSPwrLow();
 *
 */
void setACSPwrLow(void)
{
	DDRD |= ACS_PWR;
	PORTD |= ACS_PWR;
	DDRB &= ~ACS_PWRH;
	PORTB &= ~ACS_PWRH;
}

/**
 * Set ACS Power medium.
 *
 * Example:
 *
 *			setACSPwrMed();
 *
 */
void setACSPwrMed(void)
{
	DDRD &= ~ACS_PWR;
	PORTD &= ~ACS_PWR;
	DDRB |= ACS_PWRH;
	PORTB |= ACS_PWRH;
}

/**
 * Set ACS Power high.
 *
 * Example:
 *
 *			setACSPwrHigh();
 *
 */
void setACSPwrHigh(void)
{
	DDRD |= ACS_PWR;
	PORTD |= ACS_PWR;
	DDRB |= ACS_PWRH;
	PORTB |= ACS_PWRH;
}

/*****************************************************************************/
// 

void powerON(void) 
{ 
	PORTB |= PWRON;
	#ifdef POWER_ON_WARNING
		if(leds_on < 4)
			leds_on = 3;
	#endif
}

void powerOFF(void)
{
	PORTB &= ~PWRON;
	#ifdef POWER_ON_WARNING
		if(leds_on < 4)
			leds_on = (leds_on ? 1 : (statusLEDs.byte && 1));
	#endif
}

/*****************************************************************************/
// Delays, Stopwatches, Speed measurement and RC5 reception:

volatile stopwatches_t stopwatches;
volatile uint8_t delay_timer;
volatile uint8_t ms_timer;
volatile uint8_t speed_adjust_timer;

volatile uint16_t timer; // You can use this timer for everything you like!

/**
 * Timer 0 Compare ISR - This timer is used for blocking delays
 * and "Stopwatches" for non-blocking delays. 
 * Also used for Speed measurement, soft PWM Adjustment, ACS timing and 
 * RC5 code reception!
 *
 * By default, it runs at 10kHz which means this ISR is called
 * every ~100탎! This is nice for all kinds of timing stuff!
 */
ISR (TIMER0_COMP_vect)
{
	#ifdef POWER_ON_WARNING
		static uint16_t leds_on_timer = 0;
	#endif
	
	// 16bit timer (100탎 resolution)
	timer++;
	
	// Blocking delay (100탎):
	delay_timer++;
	
	// All 1ms based timing stuff
	if(ms_timer++ >= 10) { // 10 * 100탎 = 1ms
		// 16bit Stopwatches:
		if(stopwatches.watches & STOPWATCH1)
			stopwatches.watch1++;
		if(stopwatches.watches & STOPWATCH2)
			stopwatches.watch2++;
		if(stopwatches.watches & STOPWATCH3)
			stopwatches.watch3++;
		if(stopwatches.watches & STOPWATCH4)
			stopwatches.watch4++;
		if(stopwatches.watches & STOPWATCH5)
			stopwatches.watch5++;
		if(stopwatches.watches & STOPWATCH6)
			stopwatches.watch6++;
		if(stopwatches.watches & STOPWATCH7)
			stopwatches.watch7++;
		if(stopwatches.watches & STOPWATCH8)
			stopwatches.watch8++;

		// Speed measurement timer
		if(speed_timer++ > SPEED_TIMER_BASE) {
			mright_speed = mright_counter;
			mleft_speed = mleft_counter;
			mright_counter = 0;
			mleft_counter = 0;
			motor_control = true;
			speed_timer = 0;
		}
		
		// Power on LED flashing:
		#ifdef POWER_ON_WARNING
			if(leds_on < 3) {
				if(leds_on == 2) {
					if(!statusLEDs.byte) {
						if(leds_on_timer++ % 200 == 0) {
							if(leds_on_timer > POWER_ON_SHOW_TIME) {
								DDRB &= ~SL5; 
								PORTB &= ~SL5;
								leds_on = 0;
								leds_on_timer = 0;
							}
							else {
								DDRB ^= SL5; 
								PORTB ^= SL5;
							}
						}
					}
					else {
						if(!statusLEDs.LED5) {
							DDRB &= ~SL5; 
							PORTB &= ~SL5;
						}
						leds_on_timer = 0;
						leds_on = 1;
					}
				}
				else if(leds_on_timer > POWER_ON_WAIT_TIME) {
					if(leds_on == 1) {
						leds_on = 0;
						if(!statusLEDs.LED5) {
							DDRB &= ~SL5; 
							PORTB &= ~SL5;
						}
					}
					else 
						leds_on = 2;
					leds_on_timer = 0;
				}
				else
					leds_on_timer++;
			}
		#endif

		// ACS timer:
		if(acs_timer < (ACS_UPDATE_INTERVAL+1))
			acs_timer++;	

		// Overcurrent measurement timer
		if(overcurrent_timer < 55)
			overcurrent_timer++;
		
		// Bumper check timer
		if(bumper_timer < 52)
			bumper_timer++;		
		
		// Soft PWM adjustment and automatic PWM shutdown if motor power is 0:
		if(speed_adjust_timer++ > 2) {
			if(mright_ptmp != mright_power) {
				if(mright_ptmp < mright_power) 
					mright_ptmp++;
				else 
					mright_ptmp--;
				OCR1AL = mright_ptmp;
			}
			if(mleft_ptmp != mleft_power) {
				if(mleft_ptmp < mleft_power) 
					mleft_ptmp++;
				else 
					mleft_ptmp--;
				OCR1BL = mleft_ptmp;
			}
			if(mleft_ptmp || mright_ptmp)
				TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
			else
				TCCR1A = 0;
			speed_adjust_timer = 0;
		}

		ms_timer = 0;
	}

	/*****************************************************************************/
	// Nice RC5 reception code, written by Peter Dannegger.
	// Done some modifications and matched timings...
	
	#define RC5_TIME 		1.778e-3				// 1778탎
	#define RC5_PULSE_MIN	(uint8_t)(10e3 * RC5_TIME * 0.4 + 0.5)
	#define RC5_PULSE_1_2	(uint8_t)(10e3 * RC5_TIME * 0.8 + 0.5)
	#define RC5_PULSE_MAX	(uint8_t)(10e3 * RC5_TIME * 1.2 + 0.5)
	
	static uint8_t	IRCOMM_RC5_bit;		// bit value
	static uint8_t	IRCOMM_RC5_time;	// count bit time
	static uint16_t IRCOMM_RC5_tmp;		// shift bits in
	
	if((!sysStatACS.rc5_data_received)) {
		uint16_t tmp = IRCOMM_RC5_tmp;
		if(++IRCOMM_RC5_time > RC5_PULSE_MAX) {				// count pulse time
			if(!(tmp & 0x4000) && (tmp & 0x2000)) {			// only 14 bits received?
				IRCOMM_RC5_data_ok.data = tmp;				// store result
				sysStatACS.rc5_data_received = true; // we have new data!
			}
			sysStatACS.detect_rc5 = false; // NO RC5! 
			tmp = 0;
		}
		if ((IRCOMM_RC5_bit ^ PINB) & ACS) {				// change detect
			IRCOMM_RC5_bit = ~IRCOMM_RC5_bit;				// 0x00 -> 0xFF -> 0x00
			if(IRCOMM_RC5_time < RC5_PULSE_MIN)	{			// to short
				sysStatACS.detect_rc5 = false; // RC5 transmission detected! 
				tmp = 0;
			}
			if(!tmp || (IRCOMM_RC5_time > RC5_PULSE_1_2)) {	// start or long pulse time
				sysStatACS.detect_rc5 = true;
				if(!(tmp & 0x4000))							// not to many bits
					tmp <<= 1;								// shift
				if(!(IRCOMM_RC5_bit & ACS))					// inverted bit
					tmp |= 1;								// insert new bit
				IRCOMM_RC5_time = 0;						// count next pulse time
			}
		}
		IRCOMM_RC5_tmp = tmp;	
	}
	/*****************************************************************************/
	
	// Only used in Selftest program:
	#ifdef DEBUG_MEASURE_DUTY_CYCLE
		if(!isEncoderLeft())
			cycle_l_l_tmp++;
		else 
			cycle_h_l_tmp++;
		if(!isEncoderRight())
			cycle_l_r_tmp++;
		else 
			cycle_h_r_tmp++;
	#endif
}

/**
 * Delay with the help of the 10kHz timer.
 * sleep(10) delays for *about* 1ms! Not exaclty, as we do not use assembly routines
 * anywhere in this library!
 *
 * This is a blocking routine, which means that the processor
 * will loop in this routine and (except for interrupts) the
 * normal program flow is stopped!
 * Thus you should use the Stopwatch functions wherever you can!
 *
 * Example:
 *		sleep(1); // delay 1 * 100us = 100us = 0.1ms
 *		sleep(10); // delay 10 * 100us = 1000us = 1ms
 *		sleep(100); // delay 100 * 100us = 10000us = 10ms
 *		// The maximum delay is:
 *		sleep(255); // delay 255 * 100us = 25500us = 25.5ms
 */
void sleep(uint8_t time)
{
	for (delay_timer = 0; delay_timer < time;);
}

/**
 * The same as sleep() but this delays for time*1ms! 
 *
 * Example:
 *      mSleep(100); // delay 100 * 1ms = 100ms = 0.1s
 *		mSleep(1000); // delay 1000 * 1ms = 1000ms = 1s
 *
 */
void mSleep(uint16_t time)
{
	while (time--) sleep(10);
}

/**
 * Delay a number of instruction cycles. 
 * No exact delay function! And interrupts can still occur and 
 * add a lot of extra cycles! 
 * This function only guarantees to delay for a MINIMAL number
 * of cycles! 
 *
 * Example:
 * 	delayCycles(1000); // Delays for minimal 1000 instruction cycles
 *					   // (it will be a lot more...)
 */
void delayCycles(uint16_t dly)
{
	while(dly--) nop();
}

/*****************************************************************************/
// External Interrupt Output 
// Can be used to notify Master Controllers 
// about events when operating in Slave Mode.

/**
 * Set external interrupt to high level
 */
void extIntON(void)
{
	DDRA |= E_INT1;
	PORTA |= E_INT1;
}

/**
 * Set external interrupt to low level
 */
void extIntOFF(void)
{
	PORTA &= ~E_INT1;
	DDRA &= ~E_INT1;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// System control routine

/**
 * Calls all important system tasks.
 */
void task_RP6System(void)
{
	task_ADC();
	task_ACS();
	task_Bumpers();
	task_motionControl();
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


/*****************************************************************************/
// Initialisation:

/**
 * Initialise the Robot Base Controller - ALWAYS CALL THIS FIRST!
 * The Processor will not work correctly otherwise.
 * (If you don't implement your own init routine!)
 *
 * Example:
 *
 *			int16_t main(void)
 *			{
 *				initRobotBase();
 *
 *				// ... your application code
 *
 *				while(true);
 *				return 0;
 *			}
 *
 */
void initRobotBase(void)
{
	portInit();		// Setup port directions and initial values.
					// THIS IS THE MOST IMPORTANT STEP!

	cli();			// Disable global interrupts
	
	enableResetButton(); // Make sure the Reset Button is enabled!
						 // Do not disable it if you want to be able to
						 // reset your robot! (Otherwise you can only
						 // stop it by switching it off completely, 
						 // if it gets out of control ;) )

	IRCOMM_OFF(); 	     // Make sure that IRCOMM and ...
	setACSPwrOff();		 // ACS are turned OFF!

	// UART:
	UBRRH = UBRR_BAUD_LOW >> 8;	// Setup UART: Baudrate is Low Speed
	UBRRL = (uint8_t) UBRR_BAUD_LOW;
	UCSRA = 0x00;
    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
    UCSRB = (1 << TXEN) | (1 << RXEN) | (1 << RXCIE);
	
	// Initialize ADC:
	ADMUX = 0; //external reference 
	ADCSRA = (0<<ADIE) | (0<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADIF);
	SFIOR = 0;

	// Initialize External interrupts:
	MCUCR = (0 << ISC11) | (1 << ISC10) | (0 << ISC01) | (1 << ISC00);
	GICR = (1 << INT2) | (1 << INT1) | (1 << INT0);
	MCUCSR = (0 << ISC2);

	// Initialize Timer 0 -  100탎 cycle for Delays/Stopwatches, RC5 reception etc.:
	TCCR0 =   (0 << WGM00) | (1 << WGM01) 
			| (0 << COM00) | (0 << COM01) 
			| (0 << CS02)  | (1 << CS01) | (0 << CS00);
	OCR0  = 99;

	// Initialize Timer1 - PWM:
	// PWM, phase correct with ICR1 as top value.
	TCCR1A = (0 << WGM10) | (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
	TCCR1B =  (1 << WGM13) | (0 << WGM12) | (1 << CS10);
	ICR1 = 210; // Phase corret PWM top value - 210 results in 
				// about 19 kHz PWM.
				// ICR1 is the maximum (=100% duty cycle) PWM value!
				// This means that the PWM resolution is a bit lower, but
				// if the frequency is lower than 19 kHz you may hear very
				// annoying high pitch noises from the motors!
				// 19 kHz is a bit over the maximum frequency most people can
				// hear!
				// 
				// ATTENTION: Max PWM value is 210 and NOT 255 !!!
	OCR1AL = 0;
	OCR1BL = 0;
	setMotorDir(FWD,FWD); 	// Direction Forwards

	// Initialize Timer2 - ACS:
	TCCR2 = (1 << WGM21) | (0 << COM20) | (1 << CS20);
	OCR2  = 0x6E; // 0x6E = 72kHz @8MHz
	
	// Initialize Timer Interrupts:
	TIMSK = (1 << OCIE0); //| (1 << OCIE2); // Fixed: Timer2 Interrupt is turned 
	                      // off by default now! It is only active 
						  // when ACS/IRCOMM are transmitting.

	// Initialize ACS:
	sysStatACS.channel = ACS_CHANNEL_RIGHT;
	acs_state = ACS_STATE_IRCOMM_DELAY;

	sei(); // Enable Global Interrupts
}

/******************************************************************************
 * Additional info
 * ****************************************************************************
 * Changelog:
 * - v. 1.4 29.04.2008 by Dominik S. Herwald
 *      - FIX: removed "inline" attribute from some functions. Newer 
 *        WinAVR Versions don't support this attribute in GNU99 Mode anymore.
 * - v. 1.3 25.09.2007 by Dominik S. Herwald
 *		- NEW: universal timer variable with 100탎 resolution added!
 * - v. 1.2 07.08.2007 by Dominik S. Herwald
 *		- NEW: externalized ACS settings used by task_ACS function to 
 *		  RP6Config.h. Now you can improve these settings by yourself.
 *		- NEW: ACS has a small lower threshold now to reduce the 
 *		  oscillation between detected/not detected for some objects/distances.   
 * - v. 1.1 27.07.2007 by Dominik S. Herwald
 *		- NEW: added a routine to Timer0 Interrupt that flashes a LED after 
 *        some time with no active LEDs to show that the Robot is still 
 *        powered on. 
 *      - FIX: Timer2 interrupt is turned off by default now
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
