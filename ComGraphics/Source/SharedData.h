#include <vector>
#include <map>

#include "Character.h"
#include "Camera3.h"
#include "Collision.h"
#include "Interaction.h"
#include "GameState.h"

using std::vector;
using std::map;

#ifndef SHAREDDATA_H
#define SHAREDDATA_H

class SharedData
{
public:
    enum PROGRAM_STATE
    {
        PROGRAM_MENU,
        PROGRAM_GAME,
        PROGRAM_EXIT,

        PROGRAM_TOTAL
    };

    PROGRAM_STATE program_state;
    bool programstate_change;

    //Dem Gamestates
    GAME_STATE gamestate;
    LOCATION location;

    Player *player;
    Camera3 *camera;
    map<LOCATION, vector<ItemCollision>> collisionMap;
    vector<Interaction*>interactionItems;

    //monitor
    int width;
    int height;

    //cursor
    double cursor_xpos;
    double cursor_ypos;
    double cursor_newxpos;
    double cursor_newypos;

    //interaction
    bool canInteract;
    Interaction* interactptr;
	bool toiletflood;

    static SharedData* GetInstance()
    {
        static SharedData data;
        return &data;
    }

private:
    SharedData() {
        program_state = PROGRAM_MENU;
        programstate_change = false;

        player = new Player("Player");
        camera = new Camera3();
        camera->Init(Vector3(0, -140, 100), Vector3(0, -140, 110), Vector3(0, 1, 0));

        gamestate = GAME_STATE_FREE;
        location = OUTSIDE;

        cursor_xpos = 400;
        cursor_ypos = 300;
        cursor_newxpos = 400;
        cursor_newypos = 300;

		toiletflood = false;
    }
    ~SharedData() {
        delete player;
        delete camera;
    }
};

#endif /*SHAREDDATA_H*/