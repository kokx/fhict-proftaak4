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

    /* Defines */
    typedef uint8_t direction;
    #define NONE  0
    #define NORTH 1
    #define WEST  2
    #define SOUTH 4
    #define EAST  8

    /* Structures */
    struct node {
        uint8_t x;
        uint8_t y;
        struct node *north;
        struct node *west;
        struct node *south;
        struct node *east;
    };
    // by default, all directions point to a node with x = 255 and y = 255, A.K.A. "The empty node"

    /* Function prototypes */
    direction pathfinder_NextStep(direction currentDirection, uint8_t x, uint8_t y);
    void pathfinder_SetTarget(uint8_t x, uint8_t y);
    void pathfinder_init(uint8_t x, uint8_t y, direction currentDirection);

#endif
