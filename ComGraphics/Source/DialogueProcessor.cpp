#include "DialogueProcessor.h"
#include "Application.h"
#include "SharedData.h"

#include <fstream>
#include <sstream>

DialogueProcessor::DialogueProcessor() : convostate(CONVO_INTRO), npc(0), elapsedTime(0), bufferTime(0.125)
{
    map<std::string, unsigned int> loveGain;
    map<std::string, unsigned int>::iterator it;
    loveGain.insert(std::pair<std::string, unsigned int>("Chon", 0));
    loveGain.insert(std::pair<std::string, unsigned int>("Jasim", 0));
    loveGain.insert(std::pair<std::string, unsigned int>("Vee", 0));
    loveGain.insert(std::pair<std::string, unsigned int>("Wengstang", 0));

    int itemID = 0;
    int love = 0;

    std::ifstream inData;
    std::string data;
    inData.open("gifts.txt");

    if (inData.is_open()) {
        while (!inData.eof()) {
            std::getline(inData, data);

            std::stringstream dataStream(data);
            std::string line;

            //1st data - itemID
            std::getline(dataStream, line, ',');

            if (line == "#") {  //a comment
                continue;
            }
            itemID = std::stoi(line);

            //2nd data - name -- IGNORE
            std::getline(dataStream, line, ',');

            //3rd data - love gained by Chon
            std::getline(dataStream, line, ',');
            love = std::stoi(line);
            it = loveGain.find("Chon");
            it->second = love;

            //4th data - love gained by Jasim
            std::getline(dataStream, line, ',');
            love = std::stoi(line);
            it = loveGain.find("Jasim");
            it->second = love;

            //5th data - love gained by Vee
            std::getline(dataStream, line, ',');
            love = std::stoi(line);
            it = loveGain.find("Vee");
            it->second = love;

            //6th data - love gained by Wengstang
            std::getline(dataStream, line);
            love = std::stoi(line);
            it = loveGain.find("Wengstang");
            it->second = love;

            giftmap.insert(std::pair<int, map<std::string, unsigned int>>(itemID, loveGain));
        }
    }
    inData.close();
}

DialogueProcessor::~DialogueProcessor()
{
}

void DialogueProcessor::GiveGift(double dt)
{
    elapsedTime += (float)(dt);
    if (elapsedTime > bufferTime)
    {
        if ((Application::IsKeyPressed('Z')) && (SharedData::GetInstance()->player->invselect > 0))     //toggle left
        {
            SharedData::GetInstance()->player->invselect -= 1;
            elapsedTime = 0;
        }        
        if ((Application::IsKeyPressed('C')) && (SharedData::GetInstance()->player->invselect < 7))     //toggle right
        {
            SharedData::GetInstance()->player->invselect += 1;
            elapsedTime = 0;
        }
        if (Application::IsKeyPressed('X'))     //give the currently selected item
        {
            if (SharedData::GetInstance()->player->inventory[SharedData::GetInstance()->player->invselect] != 0)
            {
                elapsedTime = 0;
                int removedItemID = SharedData::GetInstance()->player->removeItem(SharedData::GetInstance()->player->invselect);
                map<int, map<std::string, unsigned int>>::iterator it = giftmap.find(removedItemID);    //gets iterator pointing to the element of the item in question
                map<std::string, unsigned int>::iterator it2 = it->second.find(npc->getName());     //gets iterator pointing to the element with love meter value
                unsigned int loveGained = it2->second;
                npc->setLoveMeter(npc->getLoveMeter() + loveGained);
                if (loveGained > 2) {
                    convostate = CONVO_GIFTHAPPY;
                }
                else {
                    convostate = CONVO_GIFTUNHAPPY;
                }
            }
        }
    }
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
            else if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 31.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 26.5f))) {
                convostate = CONVO_STARTMINIGAME;
                elapsedTime = 0;
            }
            else if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 38.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 33.5f))) {
                convostate = CONVO_COMPLIMENT;
                elapsedTime = 0;
            }
            else if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 45.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 40.5f))) {
                convostate = CONVO_GIFT;
                elapsedTime = 0;
            }
            break;
        case CONVO_NEUTRAL:
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 24.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 19.5f))) {
                SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
                elapsedTime = 0;
            }
            else if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 31.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 26.5f))) {
                convostate = CONVO_STARTMINIGAME;
                elapsedTime = 0;
            }
            else if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 38.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 33.5f))) {
                convostate = CONVO_COMPLIMENT;
                if (npc->getLoveMeter() < 20) {     //compliments can only increase love meter until 20%
                    npc->setLoveMeter(npc->getLoveMeter() + 1);
                }
                elapsedTime = 0;
            }
            else if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 45.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 40.5f))) {
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
        case CONVO_GIFTHAPPY:
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 24.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 19.5f))) {
                convostate = CONVO_NEUTRAL;
                elapsedTime = 0;
            }
            break;
        case CONVO_GIFTUNHAPPY:
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
                    SharedData::GetInstance()->veegamebool = true;
                }
                else if (npc->getName() == "Jasim") {
                    SharedData::GetInstance()->gamestate = GAME_STATE_JASIMGAME;
                    SharedData::GetInstance()->shitintoilet = true;
                    SharedData::GetInstance()->floodlevel = 0;
                }
                else if (npc->getName() == "Wengstang") {
					if (SharedData::GetInstance()->weedcounter != 0)    //weeds must reset
					{
						SharedData::GetInstance()->gamestate = GAME_STATE_WSGAME;
						SharedData::GetInstance()->weedGamebool = true;
					}
                }
                else {  //temp default
                    SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
                }
                elapsedTime = 0;
            }
            break;
        }

    }

    if (SharedData::GetInstance()->gamestate == GAME_STATE_FREE) {
        SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;
        SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;
    }
}