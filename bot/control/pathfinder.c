/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Pathfinder source file
 * ****************************************************************************
 */

#include "pathfinder.h"

/*****************************************************************************/

struct node *root;
struct node *empty;
struct node *current;

uint8_t targetX;
uint8_t targetY;

uint8_t pathfinder_NextStep(void)
{
}

void pathfinder_SetTarget(uint8_t x, uint8_t y)
{
    targetX = x;
    targetY = y;
}

void pathfinder_init(void)
{
    // initialize root node
    root = malloc(sizeof(root));

    root->x = 0;
    root->y = 0;
    
    root->north = NULL;
    root->west  = NULL;
    root->south = NULL;
    root->east  = NULL;

    // make the current node the root node
    current = root;

    // also, initialize the empty node
    empty = malloc(sizeof(empty));

    empty->x = 255;
    empty->y = 255;

    empty->north = NULL;
    empty->west  = NULL;
    empty->south = NULL;
    empty->east  = NULL;
}
