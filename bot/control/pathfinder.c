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

uint8_t nodeCount = 100;

// the target to find
uint8_t targetX;
uint8_t targetY;

direction lastDirection;

static struct node *getNextNode(struct node *node, direction dir)
{
    switch (dir) {
        case NORTH:
            return node->north;
            break;
        case EAST:
            return node->east;
            break;
        case SOUTH:
            return node->south;
            break;
        case WEST:
            return node->west;
            break;
    }
    return NULL;
}

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

    node->count = ++nodeCount;
    node->x = x;
    node->y = y;

    node->north = NULL;
    node->west  = NULL;
    node->south = NULL;
    node->east  = NULL;

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

    if ((node == empty) || (node->x > 10) || (node->y > 10) || (node == NULL)) {
        return NULL;
    }

    return node;
}

direction dirs[2];

static void getAvailableDirections(direction currentDirection)
{
    dirs[0] = NONE;
    dirs[1] = NONE;
    uint8_t num = 0;

    if (!hal_hasWallRight()) {
        dirs[num] = turnRight(currentDirection);
        num++;
    }
    if (!hal_hasWallFront()) {
        dirs[num] = currentDirection;
        num++;
    }
    if (!hal_hasWallLeft()) {
        dirs[num] = turnLeft(currentDirection);
    }

    return dirs;
}

static direction chooseDirection(direction currentDirection, struct node *node, uint8_t x, uint8_t y)
{
    getAvailableDirections(currentDirection);

    if (getNextNode(node, dirs[0]) != empty) {
        return dirs[1];
    } else if (getNextNode(node, dirs[1]) != empty) {
        return dirs[0];
    }

    uint8_t diffX = abs(x - targetX);
    uint8_t diffY = abs(y - targetY);

    if (diffX > diffY) {
        if (x > targetX) {
            if ((dirs[0] == WEST) || (dirs[1] == WEST)) {
                return WEST;
            }
        }
        if (x < targetX) {
            if ((dirs[0] == EAST) || (dirs[1] == EAST)) {
                return EAST;
            }
        }
        if (y > targetY) {
            if ((dirs[0] == NORTH) || (dirs[1] == NORTH)) {
                return NORTH;
            }
        }
        if (y < targetY) {
            if ((dirs[0] == SOUTH) || (dirs[1] == SOUTH)) {
                return SOUTH;
            }
        }
    } else {
        if (y > targetY) {
            if ((dirs[0] == NORTH) || (dirs[1] == NORTH)) {
                return NORTH;
            }
        }
        if (y < targetY) {
            if ((dirs[0] == SOUTH) || (dirs[1] == SOUTH)) {
                return SOUTH;
            }
        }
        if (x > targetX) {
            if ((dirs[0] == WEST) || (dirs[1] == WEST)) {
                return WEST;
            }
        }
        if (x < targetX) {
            if ((dirs[0] == EAST) || (dirs[1] == EAST)) {
                return EAST;
            }
        }
    }

    if (dirs[0] != NONE) {
        return dirs[0];
    }

    return currentDirection;
}

/*
 * Make a new step
 */
direction pathfinder_NextStep(direction currentDirection, uint8_t x, uint8_t y)
{
    // first check if we have reached the target already
    if ((x == targetX) && (y == targetY)) {
        // we ain't going anywhere soon!
        return NONE;
    }

    setCursorPosLCD(0, 2);
    writeIntegerLCD(root->x, DEC);
    setCursorPosLCD(0, 5);
    writeIntegerLCD(root->y, DEC);

    setCursorPosLCD(1, 0);
    writeIntegerLCD(current->count, DEC);
    setCursorPosLCD(1, 4);
    writeIntegerLCD(nodeCount, DEC);

    // now check if we can only go one direction
    if (hal_hasWallLeft() && hal_hasWallRight() && !hal_hasWallFront()) {
        return currentDirection;
    } else if (hal_hasWallLeft() && !hal_hasWallRight() && hal_hasWallFront()) {
        return turnRight(currentDirection);
    } else if (!hal_hasWallLeft() && hal_hasWallRight() && hal_hasWallFront()) {
        return turnLeft(currentDirection);
    }

    // we cannot keep cruising normally

    // get the node or create a new one
    struct node *node = getNode(current);

    if (!node) {
        node = createNode(current, currentDirection, x, y);
    }

    if (hal_hasWallLeft() && hal_hasWallRight() && hal_hasWallFront()) {
        // dead end
        current = node;
        currentDirection = turnAround(currentDirection);
    } else {
        // ok, decide where we're gonna go
        chooseDirection(currentDirection, node, x, y);
        current = node;
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

    clearLCD();

    setCursorPosLCD(0, 2);
    writeIntegerLCD(x, DEC);
    setCursorPosLCD(0, 5);
    writeIntegerLCD(y, DEC);

    mSleep(5000);

    // initialize root node
    root = malloc(sizeof(root));

    root->count = nodeCount;
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

    empty->count = 255;
    empty->x = 255;
    empty->y = 255;

    empty->north = NULL;
    empty->west  = NULL;
    empty->south = NULL;
    empty->east  = NULL;

    // initialize the current node
    if (!hal_hasWallLeft()) {
        current->west = empty;
    }
    if (!hal_hasWallFront()) {
        current->north = empty;
    }
    if (!hal_hasWallRight()) {
        current->east = empty;
    }
}
