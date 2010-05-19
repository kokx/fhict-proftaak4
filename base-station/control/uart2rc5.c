/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - ROBOT BASE EXAMPLES
 * ****************************************************************************
 * Program: Serial Interface to RC5 Interface 
 * Author(s): Corne Govers
 * Version 1.0 
 * ****************************************************************************
 * Description:
 * This program uses the RP6 library to 
 * - receive data with the serial interface and transmit it with the RC5 interface 
 * - receive data with the RC5 interface and transmit it with the serial interface
 *
 * ############################################################################
 * The Robot does NOT move in this program! You can simply put it on a table
 * next to your PC and you should connect it to the PC via the USB Interface!
 * ############################################################################
 * ****************************************************************************
 */

/*****************************************************************************/
// Includes:

#include "RP6RobotBaseLib.h" 	// The RP6 Robot Base Library.
								// Always needs to be included!


/**
 * RC5 Data reception handler - this function is called automatically from the 
 * RP6lib if new RC5 Data has been received.
 */
void receiveRC5Data(RC5data_t rc5data)
{
	// Output the received data:
	// output is iets als: "0-4-2"
	// Output getallen zijn decimaal
	writeChar(rc5data.toggle_bit + '0');
	writeString_P("-");
	writeInteger(rc5data.device, DEC);
	writeString_P("-");
	writeInteger(rc5data.key_code, DEC);
	writeChar('\n');
}


/*****************************************************************************/
// Question game

// A global variable that stores the current question:
int8_t question = 0;

/**
 * Here we receive data from the PC and transmit it to the IR. 
 * The function uses some of the reception routines of the RP6Library.
 * It is called from the main loop (s. below).
 * It's a non blocking function
 */
void receiveAndBeamup(void)
{
	if(getBufferLength()) // check if data available, if so handle it, if not: skip
	{
		RC5data_t rc5dataToSend;	// this datatype contains three members:
									// rc5dataToSend.toggle_bit		should be 0 or 1
									// rc5dataToSend.device			should be number between 0 and 31
									// rc5dataToSend.key_code		should be number between 0 and 63
		
		char rc5deviceBuffer[3];	// must also contain trailing '\0'
		char rc5codeBuffer[3];		// dito
		
		uint8_t charsToReceive = 7; // By default, we only want to receive a maximum of 8 characters
									// first character is toggle: '0' or '1'
									// second character is: '-'
									// then something between '0' and '31'	(represents 5 bit number)
									// then '-'
									// then something between '0' and '63'	(represents 6 bit number)
									// so it could be something like '0-10-40'
		
		
		char receiveBuffer[charsToReceive+1]; // our reception buffer is one byte larger
											  // than the data we want to receive, because
											  // we also need to receive the "Newline" character!
			
		clearReceptionBuffer(); // Make sure reception Buffer is empty and no junk data 
								// is left in it.
		uint8_t cnt;
		for(cnt = 0; cnt < charsToReceive+1; cnt++) {
			receiveBuffer[cnt]=0;
		}
		
		for(cnt = 0; cnt < 3; cnt++) {
			rc5deviceBuffer[cnt]=0;
			rc5codeBuffer[cnt]=0;
		}
			
		uint8_t buffer_pos = 0;
		while(true) // Loop until we received one line of Data!
		{ 
			if(getBufferLength())    // Check if we still have data (means getBufferLength() 
			{						 // is not zero)	
				receiveBuffer[buffer_pos] = readChar(); // get next character from reception buffer
				if(receiveBuffer[buffer_pos]=='\n') // End of line detected!
				{
					receiveBuffer[buffer_pos]='\0'; // Terminate String with a 0, so other routines.
					buffer_pos = 0;                 // can determine where it ends!
													// We also overwrite the Newline character here.
					break; // We are done and can leave reception loop!
				}
				else if(buffer_pos >= charsToReceive) // IMPORTANT: We can not receive more 
				{								 	  // characters than "charsToReceive" because
													  // our buffer wouldn't be large enough!
					receiveBuffer[charsToReceive]='\0';	// So if we receive more characters, we just 
														 // stop reception and terminate the String.
					break; // We are done and can leave reception loop!
				}
				buffer_pos++;			
			}										 
		}
	
		// ------------------------------------------
		// Now parse the received data:
		
		if(receiveBuffer[0] == 1) {
			rc5dataToSend.toggle_bit = 1;
		} else {
			rc5dataToSend.toggle_bit = 0;
		}
		// next part, device 
		buffer_pos = 2; 
		cnt = 0;
		while(receiveBuffer[buffer_pos] != '-') {
			rc5deviceBuffer[cnt] = receiveBuffer[buffer_pos];
			buffer_pos++; 
			cnt++;
		}
		rc5deviceBuffer[cnt] = '\0';	// vul laatste met ascii-null
		buffer_pos++; // skip second '-'
		cnt = 0;
		while(receiveBuffer[buffer_pos] != '\0') {
			rc5codeBuffer[cnt] = receiveBuffer[buffer_pos];
			buffer_pos++; 
			cnt++;
		}
		rc5codeBuffer[cnt] = '\0';

		rc5dataToSend.device = atoi(rc5deviceBuffer);
		rc5dataToSend.key_code = atoi(rc5codeBuffer);
		
		// and now: transmit the stuff
		if(rc5dataToSend.toggle_bit == 0) {
			IRCOMM_sendRC5(rc5dataToSend.device, rc5dataToSend.key_code);
		} else {
			IRCOMM_sendRC5(rc5dataToSend.device | TOGGLEBIT, rc5dataToSend.key_code);
		}
	}
}


/*****************************************************************************/
// Main function - The program starts here:

int main(void)
{
	initRobotBase(); // Always call this first! The Processor will not work
					 // correctly otherwise.

	setLEDs(0b111111); // Turn all LEDs on
	mSleep(500);       // delay 500ms
	setLEDs(0b000000); // All LEDs off
	powerON();

	// Set the RC5 Receive Handler:
	IRCOMM_setRC5DataReadyHandler(receiveRC5Data);

	// Write a message to UART:
	writeString_P("Init succeeded\n");
	
	// ---------------------------------------
	// Main loop:
	while(true)
	{
		receiveAndBeamup();
		task_RP6System(); // Motion Control tasks etc.
	}
	// End of main loop
	// ---------------------------------------

	return 0;
}

/*****************************************************************************/
// EOF
