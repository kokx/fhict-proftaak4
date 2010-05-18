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

    /* Function prototypes */
        uint8_t hal_hasWallLeft(void);
        uint8_t hal_hasWallRight(void);
        uint8_t hal_hasWallFront(void);
        void hal_turnLeft(void);
        void hal_turnRight(void);
        void hal_moveForward(void);
        void hal_init(void);

#endif
