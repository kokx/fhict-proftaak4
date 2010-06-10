/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Pathfinder header file
 * ****************************************************************************
 */

/*****************************************************************************/

#ifndef _PATHFINDER_H_
#define _PATHFINDER_H_

    /* Includes */
    #include "RP6ControlLib.h"
	#include "hal.h"

    /* Structures */
    struct listNode {
        uint8_t x;
        uint8_t y;
        struct listNode *next;
    };

    /* Defines */
    #define PATH_NORTH  (1 << 0)
    #define PATH_WEST   (1 << 1)
    #define PATH_SOUTH  (1 << 2)
    #define PATH_EAST   (1 << 3)
    #define WALL_NORTH  (1 << 4)
    #define WALL_WEST   (1 << 5)
    #define WALL_SOUTH  (1 << 6)
    #define WALL_EAST   (1 << 7)
    // by default, all directions point to a node with x = 255 and y = 255, A.K.A. "The empty node"

    /* Function prototypes */
    direction pathfinder_NextStep(direction currentDirection, uint8_t x, uint8_t y);
    void pathfinder_setTarget(uint8_t x, uint8_t y);
    void pathfinder_init(uint8_t x, uint8_t y, direction currentDirection);

#endif
