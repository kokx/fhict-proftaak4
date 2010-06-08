/* 
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Pathfinder source file
 * ****************************************************************************
 */

#include "pathfinder.h"
#include "hal.h"

/*****************************************************************************/

// pointers to some important nodes
struct node *root;
struct node *empty;
struct node *current;

// the target to find
uint8_t targetX;
uint8_t targetY;

direction lastDirection;

/*
 * Link two nodes in a specific direction
 */
static void setDirection(struct node *parent, struct node *child, direction dir)
{
    switch (dir) {
        case NORTH:
            parent->north = child;
            break;
        case EAST:
            parent->east = child;
            break;
        case SOUTH:
            parent->south = child;
            break;
        case WEST:
            parent->west = child;
            break;
    }
}

/*
 * Direction functions
 */

static direction turnLeft(direction currentDirection)
{
    switch (currentDirection) {
        case NORTH:
            return EAST;
            break;
        case EAST:
            return SOUTH;
            break;
        case SOUTH:
            return WEST;
            break;
        case WEST:
            return NORTH;
            break;
    }
    return currentDirection;
}
static direction turnRight(direction currentDirection)
{
    switch (currentDirection) {
        case NORTH:
            return WEST;
            break;
        case WEST:
            return SOUTH;
            break;
        case SOUTH:
            return EAST;
            break;
        case EAST:
            return NORTH;
            break;
    }
    return currentDirection;
}
static direction turnAround(direction currentDirection)
{
    switch (currentDirection) {
        case NORTH:
            return SOUTH;
            break;
        case WEST:
            return EAST;
            break;
        case SOUTH:
            return NORTH;
            break;
        case EAST:
            return WEST;
            break;
    }
    return currentDirection;
}

/*
 * Create us a new node
 */
static struct node *createNode(struct node *current, direction currentDirection, uint8_t x, uint8_t y)
{
    struct node *node;
    node = malloc(sizeof(node));

    node->x = x;
    node->y = y;

    node->north = NULL;
    node->west  = NULL;
    node->south = NULL;
    node->east  = NULL;

    // connect the current node to the new node
    setDirection(node, current, currentDirection);
    setDirection(current, node, lastDirection);

    return node;
}

/*
 * Get a node from the current node if it exists)
 */
static struct node *getNode(struct node *current)
{
    struct node *node = NULL;
    switch (lastDirection) {
        case NORTH:
            node = current->north;
            break;
        case WEST:
            node = current->west;
            break;
        case SOUTH:
            node = current->south;
            break;
        case EAST:
            node = current->east;
            break;
    }

    if (node != empty) {
        return node;
    }

    return NULL;
}

/*
 * Make a new step
 */
direction pathfinder_NextStep(direction currentDirection, uint8_t x, uint8_t y)
{
    // first check if we have reached the target already
    if ((current->x == targetX) && (current->y = targetY)) {
        // we ain't going anywhere soon!
        return NONE;
    }

    // now check if we can only go one direction
    if (hal_hasWallLeft() && hal_hasWallRight() && !hal_hasWallFront()) {
        return currentDirection;
    } else if (hal_hasWallLeft() && !hal_hasWallRight() && hal_hasWallFront()) {
        return turnRight(currentDirection);
    } else if (!hal_hasWallLeft() && hal_hasWallRight() && hal_hasWallFront()) {
        return turnLeft(currentDirection);
    }

    // we cannot keep cruising normally

    // create a new node
    struct node *node = getNode(current);

    if (!node) {
        node = createNode(current, currentDirection, x, y);

        // link the available directions to an empty node
        if (!hal_hasWallLeft()) {
            setDirection(node, empty, turnLeft(currentDirection));
        }
        if (!hal_hasWallRight()) {
            setDirection(node, empty, turnRight(currentDirection));
        }
        if (!hal_hasWallFront()) {
            setDirection(node, empty, currentDirection);
        }
    }

    if (hal_hasWallLeft() && hal_hasWallRight() && hal_hasWallFront()) {
        // dead end
        current = node;
        currentDirection = turnAround(currentDirection);
    } else {
        current = node;
        currentDirection = turnRight(currentDirection);
    }

    lastDirection = currentDirection;
    return currentDirection;
}

/*
 * set the target for the pathfinder to find
 */
void pathfinder_setTarget(uint8_t x, uint8_t y)
{
    targetX = x;
    targetY = y;
}

/*
 * Initialize the pathfinder
 */
void pathfinder_init(uint8_t x, uint8_t y, direction currentDirection)
{
    lastDirection = currentDirection;

    // initialize root node
    root = malloc(sizeof(root));

    root->x = x;
    root->y = y;

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
