#include "Interaction.h"
#include <string>
#include <map>

using std::map;

#ifndef DIALOGUE_PROCESSOR_H
#define DIALOGUE_PROCESSOR_H

enum NPC_TALKINGTO
{
    NPC_CHON,
    NPC_WENGSTANG,
    NPC_VEE,
    NPC_JASIM,
    //NPC_ADOLPH,
    NPC_TOTAL
};

enum CONVO_STATE
{
    CONVO_INTRO,
    CONVO_GIFT,
    //CONVO_GIFTHAPPY,
    //CONVO_GIFTUNHAPPY,
    CONVO_STARTMINIGAME,
    CONVO_TOTAL
};

class DialogueProcessor
{
public:
    DialogueProcessor();    //initialise map here
    ~DialogueProcessor();
    NPC_TALKINGTO NPCstate;
    CONVO_STATE convostate;
    map<NPC_TALKINGTO, map<CONVO_STATE, std::string>> dialogues;
    //map<NPC_TALKINGTO, map<CONVO_STATE, std::vector>> dialogues;
    std::string getDialogue();  //process states to get the corresponding dialogue
};

#endif