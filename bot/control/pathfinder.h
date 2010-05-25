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

    /* Defines */
    #define NORTH 0
    #define WEST  1
    #define SOUTH 2
    #define EAST  3

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

    /* Includes */

    /* Function prototypes */
    uint8_t pathfinder_NextStep(void);
    void pathfinder_SetTarget(uint8_t x, uint8_t y);
    void pathfinder_init(void);

#endif
