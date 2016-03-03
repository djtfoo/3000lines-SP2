/******************************************************************************/
/*!
\file	DialogueProcessor.h
\author 3000 Lines Studio
\brief
DialogueProcessor header file
*/
/******************************************************************************/

#include "Interaction.h"
#include "Character.h"
#include "Button.h"

#include <string>
#include <map>

using std::map;

#ifndef DIALOGUE_PROCESSOR_H
#define DIALOGUE_PROCESSOR_H

/*enum NPC_TALKINGTO
{
    NPC_CHON,
    NPC_WENGSTANG,
    NPC_VEE,
    NPC_JASIM,
    //NPC_ADOLPH,
    NPC_TOTAL
};*/

/******************************************************************************/
/*!
Class DialogueProcessor:
\brief	Stores information for dialogue
*/
/******************************************************************************/
class DialogueProcessor
{
    double elapsedTime, bufferTime;

    Button b_Gift;
    Button b_Compliment;
    Button b_MiniGame;
    Button b_Back;

public:
    DialogueProcessor();
    ~DialogueProcessor();
    CONVO_STATE convostate;
    NPC* npc;

    map<int, map<std::string, int>> giftmap;

    void GiveGift(double dt);
    void Compliment();
    void CheckCursor(double dt);
};

#endif