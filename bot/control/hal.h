/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * HAL header file
 * ****************************************************************************
 */

/*****************************************************************************/

#ifndef _HAL_H_
#define _HAL_H_

    /* Includes */
	#include "RP6ControlLib.h" 		// The RP6 Control Library. 
								// Always needs to be included!

	#include "RP6I2CmasterTWI.h"	// I2C Master Library
	#include "RP6I2CMasterLibExt.h"
	
	/* Defines */
    typedef uint8_t direction;
    #define NONE  0
    #define NORTH 1
    #define WEST  2
    #define SOUTH 4
    #define EAST  8

    /* Function prototypes */
        uint8_t hal_hasWallLeft(void);
        uint8_t hal_hasWallRight(void);
        uint8_t hal_hasWallFront(void);
        void hal_turnLeft(void);
        void hal_turnRight(void);
        void hal_moveForward(void);
        void hal_init(void);
		void hal_check(void);

#endif
