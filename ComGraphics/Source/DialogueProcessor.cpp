#include "DialogueProcessor.h"
#include "Application.h"
#include "SharedData.h"

DialogueProcessor::DialogueProcessor() : convostate(CONVO_INTRO), npc(0), elapsedTime(0), bufferTime(0.125)
{
}

DialogueProcessor::~DialogueProcessor()
{
}

void DialogueProcessor::CheckCursor(double dt)
{
    elapsedTime += dt;

    if (Application::IsKeyPressed(VK_LBUTTON) && elapsedTime > bufferTime && SharedData::GetInstance()->cursor_newxpos >= (SharedData::GetInstance()->width * 0.625f) && SharedData::GetInstance()->cursor_newxpos <= SharedData::GetInstance()->width) {
        switch (convostate)
        {
        case CONVO_INTRO:
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 24.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 19.5f))) {
                SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
                elapsedTime = 0;
            }
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 31.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 26.5f))) {
                convostate = CONVO_STARTMINIGAME;
                elapsedTime = 0;
            }
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 38.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 33.5f))) {
                convostate = CONVO_COMPLIMENT;
                elapsedTime = 0;
            }
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 45.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 40.5f))) {
                convostate = CONVO_GIFT;
                elapsedTime = 0;
            }
            break;
        case CONVO_NEUTRAL:
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 24.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 19.5f))) {
                SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
                elapsedTime = 0;
            }
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 31.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 26.5f))) {
                convostate = CONVO_STARTMINIGAME;
                elapsedTime = 0;
            }
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 38.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 33.5f))) {
                convostate = CONVO_COMPLIMENT;
                elapsedTime = 0;
            }
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 45.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 40.5f))) {
                convostate = CONVO_GIFT;
                elapsedTime = 0;
            }
            break;
        case CONVO_GIFT:
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 24.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 19.5f))) {
                convostate = CONVO_NEUTRAL;
                elapsedTime = 0;
            }
            break;
        case CONVO_COMPLIMENT:
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 24.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 19.5f))) {
                convostate = CONVO_NEUTRAL;
                elapsedTime = 0;
            }
            break;
        case CONVO_STARTMINIGAME:
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 24.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 19.5f))) {
                if (npc->getName() == "Chon") {
                    SharedData::GetInstance()->gamestate = GAME_STATE_CHONGAME;
                    SharedData::GetInstance()->chonGamebool = true;
                }
                else if (npc->getName() == "Vee") {
                    SharedData::GetInstance()->gamestate = GAME_STATE_VEEGAME;
                    SharedData::GetInstance()->chonGamebool = true;
                }
                else if (npc->getName() == "Jasim") {
                    SharedData::GetInstance()->gamestate = GAME_STATE_JASIMGAME;
                    SharedData::GetInstance()->chonGamebool = true;
                }
                else if (npc->getName() == "Wengstang") {
                    SharedData::GetInstance()->gamestate = GAME_STATE_WSGAME;
                    SharedData::GetInstance()->weedGamebool = true;
                }
                else {  //temp
                    SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
                }
                SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;
                SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;
                elapsedTime = 0;
            }
            break;
        }

    }
}