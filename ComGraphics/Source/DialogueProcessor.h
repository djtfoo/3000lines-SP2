#include "Interaction.h"
#include "Character.h"
#include "GameState.h"
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

class DialogueProcessor
{
    double elapsedTime, bufferTime;

public:
    DialogueProcessor();
    ~DialogueProcessor();
    CONVO_STATE convostate;
    NPC* npc;

    void CheckCursor(double dt);
};

#endif