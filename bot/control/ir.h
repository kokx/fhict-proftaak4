/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * IR header file
 * ****************************************************************************
 */

/*****************************************************************************/

#ifndef _IR_H_
#define _IR_H_

    struct ir_data {
        uint8_t targetX;
        uint8_t targetY;
        uint8_t currentX;
        uint8_t currentY;
    };
    /* Includes */

    /* Function prototypes */
    uint8_t ir_getStartX(void);
    uint8_t ir_getStartY(void);
    uint8_t ir_getTargetX(void);
    uint8_t ir_getTargetY(void);
    void ir_sendBaseStation(void);
    void ir_receiveBaseStation(void);
    void ir_sendSituation(direction dir, uint8_t x, uint8_t y);
	void ir_init(void);
#endif
