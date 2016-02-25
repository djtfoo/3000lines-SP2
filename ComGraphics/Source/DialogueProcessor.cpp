#include "DialogueProcessor.h"
#include "Application.h"
#include "SharedData.h"

DialogueProcessor::DialogueProcessor() : npc(0), convostate(CONVO_INTRO)
{
}

DialogueProcessor::~DialogueProcessor()
{
}

void DialogueProcessor::CheckCursor()
{
    if (Application::IsKeyPressed(VK_LBUTTON)) {
        switch (convostate)
        {
        case CONVO_INTRO:
            break;
        case CONVO_NEUTRAL:
            break;
        case CONVO_GIFT:
            break;
        case CONVO_COMPLIMENT:
            break;
        case CONVO_STARTMINIGAME:
            break;
        }

    }
}