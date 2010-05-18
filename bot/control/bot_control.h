/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Bot control header file
 * ****************************************************************************
 */

/*****************************************************************************/

#ifndef _BOT_CONTROL_H_
#define _BOT_CONTROL_H_

    /* Includes */
        #include "RP6ControlLib.h"
        #include "RP6I2CmasterTWI.h"

    /* Function prototypes */
        void I2C_transmissionError(uint8_t errorState);
        void main(void);

#endif
