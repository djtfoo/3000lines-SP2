#include <vector>

#include "Character.h"
#include "Camera3.h"
#include "Collision.h"
#include "Interaction.h"
#include "GameState.h"

using std::vector;

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

    Player *player;
    Camera3 *camera;
    vector<ItemCollision>collisionItems;
    vector<Interaction*>interactionItems;

    //cursor
    double cursor_xpos;
    double cursor_ypos;
    double cursor_newxpos;
    double cursor_newypos;

    //interaction
    bool canInteract;
    Interaction* interactptr;

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

        cursor_xpos = 400;
        cursor_ypos = 300;
        cursor_newxpos = 400;
        cursor_newypos = 300;
    }
    ~SharedData() {
        delete player;
        delete camera;
    }
};

#endif /*SHAREDDATA_H*/