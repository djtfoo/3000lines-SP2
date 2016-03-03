/******************************************************************************/
/*!
\file	SharedData.h
\author 3000 Lines Studio
\brief
SharedData header file
*/
/******************************************************************************/

#include <vector>
#include <map>

#include "Character.h"
#include "Camera3.h"
#include "Collision.h"
#include "Interaction.h"
#include "GameState.h"
#include "DialogueProcessor.h"

#include "irrklang/irrKlang.h"

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")    //link with irrKlang.dll

using std::vector;
using std::map;

#ifndef SHAREDDATA_H
#define SHAREDDATA_H

/******************************************************************************/
/*!
Class SharedData:
\brief	Stores data to be used across all files
*/
/******************************************************************************/
class SharedData
{
public:
    //start sound engine with default parameters
    ISoundEngine* engine;
    bool playSound;
    bool playMusic;

    PROGRAM_STATE program_state;
    bool programstate_change;
	float daynighttime;
    int daynumber;
	int pointscounter;
	int weedcounter;

    //Dem Gamestates
    GAME_STATE gamestate;
    LOCATION location;
    bool paused;

    Player *player;
    Enemy *enemy;
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
    char interactbutton;    //changes for toilet game only
    int firstball;
    bool ballDoneMoving;
    bool ballpickup;    //chon ballgame
    bool ballgameComplete;

    //Puzzle Interaction
    float one, two, three, four;
    int switchCount;
    bool switch1, switch2, switch3, switch4;
    bool switchFlip;

    //toilet interaction
    bool shitintoilet;
    float floodlevel;
    double timeElapsed;

    //bed interaction
    bool sleep;

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
        //irrKlang sound engine
        engine = createIrrKlangDevice();
        playSound = true;
        playMusic = true;

        program_state = PROGRAM_MENU;
        programstate_change = false;

        player = new Player("Player");
        enemy = new Enemy("Rabbit", Vector3(5000, 25, 5000));
        camera = new Camera3();
        camera->Init(Vector3(0, 25, 0), Vector3(1, 25, 0), Vector3(0, 1, 0));

        //NPCs
        NPC* npc = new NPC("Chon", Vector3(412, 20, -459), "ChonDialogue.txt");
        NPCs.push_back(npc);
        npc = new NPC("Vee", Vector3(600, 10, 440), "VeeDialogue.txt");
        NPCs.push_back(npc);
        npc = new NPC("Jasim", Vector3(685, 10, -430), "JasimDialogue.txt");
        NPCs.push_back(npc);
        npc = new NPC("Wengstang", Vector3(950, 10, -460), "WengstangDialogue.txt");
        NPCs.push_back(npc);

        interactptr = 0;

        //daynight
        daynighttime = 0;
        daynumber = 1;

        //states
        gamestate = GAME_STATE_FREE;
        location = OUTSIDE;
        paused = false;
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
        shitintoilet = false;
        floodlevel = -2;
        timeElapsed = 0;    //for Jasim and Chon mini-games
        interactbutton = 'E';
        
        //Chon mini-game (lab)
        ballpickup = false;     //chon ballgame
        ballgameComplete = false;

		//weed mini-game
		pointscounter = 0;
		weedcounter = 10;
		
        //bed interaction
        sleep = false;

        firstball = 0;
        ballDoneMoving = true;
    }
    ~SharedData() {
        engine->drop();     //delete irrKlang engine

        delete player;
        delete enemy;
        delete camera;

        for (size_t i = 0; i < NPCs.size(); ++i) {
            delete NPCs[i];
        }
    }
};

#endif /*SHAREDDATA_H*/