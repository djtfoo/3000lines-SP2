#ifndef GAMESTATE_H
#define GAMESTATE_H


enum GAME_STATE
{
    GAME_STATE_FREE = 0,
    GAME_STATE_SHOP,
    GAME_STATE_WSGAME,
    GAME_STATE_CHONGAME,
    GAME_STATE_VEEGAME,
    GAME_STATE_PAUSED,
    GAME_STATE_RABBIT,

    GAME_STATE_TOTAL,
};

enum LOCATION
{
    OUTSIDE,
    HANGAR,
    CORRIDOR,   //can split corridor into different segments
    SHOP,
    LABORATORY,
    CONTROLROOM,
    CANTEEN,
    TOILET,
    ROOM1,
    ROOM2,
    ROOM3,
    ROOM4,

    LOCATION_TOTAL
};

//GAME_STATE gamestate;

#endif /* GAMESTATE_H */