/******************************************************************************/
/*!
\file	GameState.h
\author 3000 Lines Studio
\brief
Gamestate header file
*/
/******************************************************************************/

#ifndef GAMESTATE_H
#define GAMESTATE_H

/******************************************************************************/
/*!
Enum PROGRAM_STATE:
\brief	Stores the different states for the program
*/
/******************************************************************************/
enum PROGRAM_STATE
{
    PROGRAM_MENU,
    PROGRAM_GAME,
    PROGRAM_EXIT,

    PROGRAM_TOTAL
};

/******************************************************************************/
/*!
Enum GAME_STATE:
\brief	Stores the different states for the game
*/
/******************************************************************************/
enum GAME_STATE
{
    GAME_STATE_FREE = 0,
    GAME_STATE_SHOP,
    GAME_STATE_WSGAME,
    GAME_STATE_CHONGAME,
    GAME_STATE_VEEGAME,
    GAME_STATE_JASIMGAME,
    //GAME_STATE_PAUSED,
    GAME_STATE_RABBIT,
    GAME_STATE_DIALOGUE,

    GAME_STATE_TOTAL,
};

/******************************************************************************/
/*!
Enum CONVO_STATE:
\brief	Stores the different states for the conversations
*/
/******************************************************************************/
enum CONVO_STATE
{
    CONVO_INTRO,    //0
    CONVO_NEUTRAL,  //1
    CONVO_GIFT,     //2
    CONVO_GIFTHAPPY,    //3
    CONVO_GIFTUNHAPPY,  //4
    CONVO_COMPLIMENT,   //5
    CONVO_STARTMINIGAME,    //6
    CONVO_FINISHMINIGAME,   //7
    CONVO_BOSSPROMPT,   //8
    CONVO_TOTAL
};

/******************************************************************************/
/*!
Enum LOCATION:
\brief	Stores the different locations
*/
/******************************************************************************/
enum LOCATION
{
    OUTSIDE,    //0
    HANGAR,     //1
    CORRIDOR,   //2
    SHOP,       //3
    LABORATORY, //4
    CONTROLROOM,    //5
    CANTEEN,    //6
    TOILET,     //7
    ROOM1,      //8
    ROOM2,      //9
    ROOM3,      //10
    ROOM4,      //11
    PLANTATION, //12

    LOCATION_TOTAL
};

//GAME_STATE gamestate;

#endif /* GAMESTATE_H */