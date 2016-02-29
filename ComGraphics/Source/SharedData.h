#include <vector>
#include <map>

#include "Character.h"
#include "Camera3.h"
#include "Collision.h"
#include "Interaction.h"
#include "GameState.h"
#include "DialogueProcessor.h"

using std::vector;
using std::map;

#ifndef SHAREDDATA_H
#define SHAREDDATA_H

class SharedData
{
public:
    PROGRAM_STATE program_state;
    bool programstate_change;
	float daynighttime;

    //Dem Gamestates
    GAME_STATE gamestate;
    LOCATION location;

    Player *player;
    Camera3 *camera;
    map<LOCATION, vector<ItemCollision>> collisionMap;
    vector<NPC*> NPCs;
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
	int interactnumber;
    Interaction* interactptr;
    bool gatelookat;
    bool gateopen;
	bool toiletflood;
    bool chonGamebool;
    bool ballpickup;    //chon ballgame

    //Puzzle Interaction
    float one, two, three, four, switchCount;
    bool switch1, switch2, switch3, switch4;
    bool switchFlip;

    DialogueProcessor dialogueProcessor;

    void CheckCursor()  //applicable when in dialogue, pause, shop, menu
    {
        if (SharedData::GetInstance()->cursor_newxpos != SharedData::GetInstance()->cursor_xpos) {
            double diff_xpos = SharedData::GetInstance()->cursor_xpos - SharedData::GetInstance()->cursor_newxpos;
            SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;
        }

        if (SharedData::GetInstance()->cursor_newypos != SharedData::GetInstance()->cursor_ypos) {
            double diff_ypos = SharedData::GetInstance()->cursor_ypos - SharedData::GetInstance()->cursor_newypos;
            SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;
        }
    }

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

        //NPCs
        NPC* npc = new NPC("Chon", Vector3(412, 5, -459), "ChonDialogue.txt");
        NPCs.push_back(npc);
        npc = new NPC("Vee", Vector3(600, 10, 440), "VeeDialogue.txt");
        NPCs.push_back(npc);
        npc = new NPC("Jasim", Vector3(685, 0, -430), "JasimDialogue.txt");
        NPCs.push_back(npc);
        npc = new NPC("Wengstang", Vector3(950, 0, -460), "WengstangDialogue.txt");
        NPCs.push_back(npc);

        interactptr = 0;

        //states
        gamestate = GAME_STATE_FREE;
        location = OUTSIDE;
        gateopen = false;

        //cursor for main menu
        cursor_xpos = 400;
        cursor_ypos = 300;
        cursor_newxpos = 400;
        cursor_newypos = 300;

        //Vee mini-game (light puzzle)
        one = two = three = four = 1;
        switchCount = 0;
        switch1 = switch2 = switch3 = switch4 = false;
        switchFlip = false;

        //toilet
		toiletflood = false;
        
        //Chon mini-game (lab)
        chonGamebool = false;
        ballpickup = false;     //chon ballgame
    }
    ~SharedData() {
        delete player;
        delete camera;
    }
};

#endif /*SHAREDDATA_H*/