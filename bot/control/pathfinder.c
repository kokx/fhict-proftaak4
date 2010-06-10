/*
 * ****************************************************************************
 * RP6 ROBOT SYSTEM - RP6 CONTROL M32 Examples
 * ****************************************************************************
 * Pathfinder source file
 * ****************************************************************************
 */

#include "pathfinder.h"
#include "hal.h"

uint8_t targetX;
uint8_t targetY;
uint8_t finalTargetX;
uint8_t finalTargetY;

// map[x][y]
uint8_t map[7][9];

struct listNode *lastNode;

/*****************************************************************************/

/*
 * Linked list functions
 */

static void list_addNode(uint8_t x, uint8_t y)
{
    struct listNode *new;
    new = malloc(sizeof(struct listNode));

    if (!new) {
        showScreenLCD("################", " Memory Error");
    }

    new->x = x;
    new->y = y;

    new->next = lastNode;

    lastNode = new;
}

/*
 * Check the walls config
 */
static uint8_t checkWalls(uint8_t x, uint8_t y)
{
    if (!(map[x][y] & PATH_NORTH) && !(map[x][y] & WALL_NORTH)) {
        return true;
    }
    if (!(map[x][y] & PATH_WEST) && !(map[x][y] & WALL_WEST)) {
        return true;
    }
    if (!(map[x][y] & PATH_SOUTH) && !(map[x][y] & WALL_SOUTH)) {
        return true;
    }
    if (!(map[x][y] & PATH_EAST) && !(map[x][y] & WALL_EAST)) {
        return true;
    }
    return false;
}

// temp function
static void writeDir(direction dir, uint8_t num)
{
    setCursorPosLCD(1, num * 2);

    switch (dir) {
        case NORTH:
            writeCharLCD('N');
            break;
        case WEST:
            writeCharLCD('W');
            break;
        case SOUTH:
            writeCharLCD('S');
            break;
        case EAST:
            writeCharLCD('E');
            break;
        case NONE:
            writeCharLCD('X');
            break;
        default:
            writeIntegerLCD(dir, DEC);
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
            return WEST;
            break;
        case EAST:
            return NORTH;
            break;
        case SOUTH:
            return EAST;
            break;
        case WEST:
            return SOUTH;
            break;
    }
    return currentDirection;
}
static direction turnRight(direction currentDirection)
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
 * Method to get available directions
 */
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
}

static void checkAvailableDirections(uint8_t x, uint8_t y, uint8_t num)
{
    if ((targetX != finalTargetX) || (targetY != finalTargetY)) {
        return;
    }

    switch (dirs[num]) {
        case NORTH:
            if (!(map[x][y] & PATH_NORTH)) {
                return;
            }
            break;
        case WEST:
            if (!(map[x][y] & PATH_WEST)) {
                return;
            }
            break;
        case SOUTH:
            if (!(map[x][y] & PATH_SOUTH)) {
                return;
            }
            break;
        case EAST:
            if (!(map[x][y] & PATH_EAST)) {
                return;
            }
            break;
    }
    dirs[num] = NONE;
}

static void savePosition(uint8_t x, uint8_t y, direction dir)
{
    switch (dir) {
        case NORTH:
            map[x][y] |= PATH_NORTH;
            break;
        case WEST:
            map[x][y] |= PATH_WEST;
            break;
        case SOUTH:
            map[x][y] |= PATH_SOUTH;
            break;
        case EAST:
            map[x][y] |= PATH_EAST;
            break;
    }
}

static void saveWall(uint8_t x, uint8_t y, direction dir)
{
    switch (dir) {
        case NORTH:
            map[x][y] |= WALL_NORTH;
            break;
        case WEST:
            map[x][y] |= WALL_WEST;
            break;
        case SOUTH:
            map[x][y] |= WALL_SOUTH;
            break;
        case EAST:
            map[x][y] |= WALL_EAST;
            break;
    }
}

// temp debug function
static printLinkedList()
{
    struct listNode *node;

    node = lastNode;

    do {
        setCursorPosLCD(0, 4);
        writeCharLCD('X');

        setCursorPosLCD(0, 6);
        writeIntegerLCD(node->x, DEC);

        setCursorPosLCD(0, 8);
        writeCharLCD('Y');

        setCursorPosLCD(0, 10);
        writeIntegerLCD(node->y, DEC);

        mSleep(300);
    } while (node = node->next);
}

/*
 * Make a new step
 */
direction pathfinder_NextStep(direction currentDirection, uint8_t x, uint8_t y)
{
    // first check if we have reached the target already
    if ((x == targetX) && (y == targetY)) {
        if ((targetX == finalTargetX) && (targetY == finalTargetY)) {
            // we ain't going anywhere soon!
            return NONE;
        }
        targetX = finalTargetX;
        targetY = finalTargetY;
    }

    // save the walls
    if (hal_hasWallLeft()) {
        saveWall(x, y, turnLeft(currentDirection));
    }
    if (hal_hasWallFront()) {
        saveWall(x, y, currentDirection);
    }
    if (hal_hasWallRight()) {
        saveWall(x, y, turnRight(currentDirection));
    }

    // print walls config
    // printLinkedList();

    // save the current position
    savePosition(x, y, turnAround(currentDirection));

    // now check if we can only go one direction
    if (hal_hasWallLeft() && hal_hasWallRight() && !hal_hasWallFront()) {
        return currentDirection;
    } else if (hal_hasWallLeft() && !hal_hasWallRight() && hal_hasWallFront()) {
        return turnRight(currentDirection);
    } else if (!hal_hasWallLeft() && hal_hasWallRight() && hal_hasWallFront()) {
        return turnLeft(currentDirection);
    }

    // first check if we are at a dead end
    if (hal_hasWallLeft() && hal_hasWallFront() && hal_hasWallRight()) {
        return turnAround(currentDirection);
    }

    list_addNode(x, y);

    // now we need to make a slightly more complicated decision
    uint8_t diffX = abs(x - targetX);
    uint8_t diffY = abs(y - targetY);

    setCursorPosLCD(0, 13);
    writeCharLCD('X');
    setCursorPosLCD(1, 13);
    writeCharLCD('Y');

    setCursorPosLCD(0, 15);
    writeIntegerLCD(x, DEC);
    setCursorPosLCD(1, 15);
    writeIntegerLCD(y, DEC);

    // get the best order of directions
    direction bestOrder[4];

    if (diffX > diffY) {
        // we want to go WEST or EAST
        if (x > targetX) {
            bestOrder[0] = WEST;
            bestOrder[3] = EAST;
        } else {
            bestOrder[0] = EAST;
            bestOrder[3] = WEST;
        }
        if (y > targetY) {
            bestOrder[1] = NORTH;
            bestOrder[2] = SOUTH;
        } else {
            bestOrder[1] = SOUTH;
            bestOrder[2] = NORTH;
        }
    } else {
        // we want to go NORTH or SOUTH
        if (y > targetY) {
            bestOrder[0] = NORTH;
            bestOrder[3] = SOUTH;
        } else {
            bestOrder[0] = SOUTH;
            bestOrder[3] = NORTH;
        }
        if (x > targetX) {
            bestOrder[1] = WEST;
            bestOrder[2] = EAST;
        } else {
            bestOrder[1] = EAST;
            bestOrder[2] = WEST;
        }
    }

    // get the available directions
    getAvailableDirections(currentDirection);

    // check the available directions
    for (int i = 0; i < 4; i++) {
        //writeDir(bestOrder[i], i);

        if (bestOrder[i] == dirs[0]) {
            checkAvailableDirections(x, y, 0);
            bestOrder[i] = dirs[0];
        } else if (bestOrder[i] == dirs[1]) {
            checkAvailableDirections(x, y, 1);
            bestOrder[i] = dirs[1];
        } else {
            continue;
        }

        if (bestOrder[i] != NONE) {
            savePosition(x, y, bestOrder[i]);
            return bestOrder[i];
        }
    }

    // we had everything, temporary set the target to an old place,
    // but only if we aren't already going to a temporary target
    if ((targetX == finalTargetX) && (targetY == finalTargetY)) {
        // traverse through the linked list until we found a suitable target
        struct listNode *node;

        node = lastNode;

        do {
            // check if we had all walls here
            if (checkWalls(node->x, node->y)) {
                targetX = node->x;
                targetY = node->y;
                break;
            }
        } while (node = node->next);

        setCursorPosLCD(0, 4);
        writeCharLCD('X');

        setCursorPosLCD(0, 6);
        writeIntegerLCD(node->x, DEC);

        setCursorPosLCD(0, 8);
        writeCharLCD('Y');

        setCursorPosLCD(0, 10);
        writeIntegerLCD(node->y, DEC);

        mSleep(300);
        return pathfinder_NextStep(x, y, currentDirection);
    } else if (hal_hasWallFront()) {
        // if all else fails.....
        return turnRight(currentDirection);
    }

    return currentDirection;
}

/*
 * set the target for the pathfinder to find
 */
void pathfinder_setTarget(uint8_t x, uint8_t y)
{
    finalTargetX = x;
    finalTargetY = y;

    targetX = x;
    targetY = y;

    // free LOTS of memory if needed
    //struct listNode *node;
}

/*
 * Initialize the pathfinder
 */
void pathfinder_init(uint8_t x, uint8_t y, direction currentDirection)
{
    lastNode = malloc(sizeof(struct listNode));

    lastNode->x = x;
    lastNode->y = y;

    lastNode->next = NULL;
}
