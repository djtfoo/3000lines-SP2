/******************************************************************************/
/*!
\file	Collision.h
\author 3000 Lines Studio
\brief
Collision header file
*/
/******************************************************************************/

#ifndef WALLCOLLISION_H
#define WALLCOLLISION_H

/******************************************************************************/
/*!
Enum WALL_DIR:
\brief	which direction you are walking towards to collide with the wall
*/
/******************************************************************************/
enum WALL_DIR   //which direction you are walking towards to collide with the wall
{
    WALL_LEFT = 0,  //when walking towards -ve x-axis direction
    WALL_RIGHT,     //when walking towards +ve x-axis direction
    WALL_FRONT,     //when walking towards -ve z-axis direction
    WALL_BACK,      //when walking towards +ve z-axis direction
    WALL_TOP,       //when walking towards +ve y-axis direction
    WALL_BOTTOM,    //when walking towards -ve y-axis direction
    WALL_TOTAL
};

/******************************************************************************/
/*!
Enum STAIR_DIR:
\brief	stair locations
*/
/******************************************************************************/
enum STAIR_DIR
{
    STAIR_X,        //when stairs is along x-axis direction
    STAIR_Z,        //when stairs is along z-axis direction
    STAIR_TOTAL
};

/******************************************************************************/
/*!
Enum DOOR_STATE:
\brief	what state the door is in
*/
/******************************************************************************/
enum DOOR_STATE
{
    DOOR_OPEN,
    DOOR_OPENING,
    DOOR_CLOSE,
    DOOR_CLOSING,
    DOOR_STATES
};

/******************************************************************************/
/*!
Struct WallCollision:
\brief	Contains information for walls for collision
*/
/******************************************************************************/
struct WallCollision
{
    WALL_DIR dir;
    float coordinate;
};

/******************************************************************************/
/*!
Struct ItemsCollision:
\brief	Contains information for items for collision
*/
/******************************************************************************/
struct ItemCollision
{
    float minX, maxX, minZ, maxZ;
};

/******************************************************************************/
/*!
Struct StairsCollision:
\brief	Contains information for stairs for collision
*/
/******************************************************************************/
struct StairsCollision
{
    STAIR_DIR dir;
    float minVal, maxVal;
    float minY, maxY;
    int steps;      //number of steps
    float incrementVal, incrementY;
};

/*struct ItemInteraction
{
    float minX, maxX, minY, maxY, minZ, maxZ;
};*/

/******************************************************************************/
/*!
Struct FlightCollision:
\brief	Contains information for flight for collision
*/
/******************************************************************************/
struct FlightCollision
{
    float minX, maxX, minY, maxY, minZ, maxZ;
};

/******************************************************************************/
/*!
Struct Hitbox:
\brief	Contains information for hitbox
*/
/******************************************************************************/
struct Hitbox
{
    float minX, maxX, minY, maxY, minZ, maxZ;
};

/******************************************************************************/
/*!
Struct Door:
\brief	Contains information for doors
*/
/******************************************************************************/
struct Door
{
    DOOR_STATE state;
    int index;
    int direction;  //1 for opening, -1 for closing
    float change;
    Door() {
        state = DOOR_CLOSE;
        direction = 1;
        change = 0;
    }
};

#endif