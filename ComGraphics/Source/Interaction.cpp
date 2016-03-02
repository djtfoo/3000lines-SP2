#include "Interaction.h"
#include "SharedData.h"

//abstract class of interaction
Interaction::Interaction(bool pressbutton) : pressButton(pressbutton)
{
}

Interaction::~Interaction()
{
}

//Gate interaction open/close
GateInteraction::GateInteraction() : Interaction(false)
{
}
GateInteraction::~GateInteraction()
{
}

void GateInteraction::DoInteraction()
{
    if (SharedData::GetInstance()->canInteract)
    {
        SharedData::GetInstance()->gateopen = true;
    }
    else
    {
        SharedData::GetInstance()->gateopen = false;
    }
}


//pipe puzzle interaction (rotation)
PipePuzzleInteraction::PipePuzzleInteraction() : Interaction(true)
{
}

PipePuzzleInteraction::~PipePuzzleInteraction()
{
}

void PipePuzzleInteraction::DoInteraction()
{
}

//Dialogue Interaction class
Dialogue::Dialogue() : Interaction(true)
{
}

Dialogue::~Dialogue()
{
}

void Dialogue::DoInteraction()
{
    if (SharedData::GetInstance()->gamestate == GAME_STATE_FREE) {
        for (vector<NPC*>::iterator it = SharedData::GetInstance()->NPCs.begin(); it != SharedData::GetInstance()->NPCs.end(); ++it) {
            Vector3 b1 = SharedData::GetInstance()->interactptr->bound1;
            Vector3 b2 = SharedData::GetInstance()->interactptr->bound2;
            if ((*it)->position_.x >= b1.x && (*it)->position_.x <= b2.x &&
                (*it)->position_.y >= b1.y && (*it)->position_.y <= b2.y &&
                (*it)->position_.z >= b1.z && (*it)->position_.z <= b2.z) {
                SharedData::GetInstance()->dialogueProcessor.npc = *it;
                SharedData::GetInstance()->dialogueProcessor.convostate = CONVO_INTRO;
                SharedData::GetInstance()->gamestate = GAME_STATE_DIALOGUE;

                //set cursor
                SharedData::GetInstance()->cursor_xpos = 400;
                SharedData::GetInstance()->cursor_ypos = 400;
                SharedData::GetInstance()->cursor_newxpos = 400;
                SharedData::GetInstance()->cursor_newypos = 400;
                break;
            }
        }
    }

}

//shop interaction (enter shop state)
ShopInteraction::ShopInteraction() : Interaction(true)
{
}

ShopInteraction::~ShopInteraction()
{
}

void ShopInteraction::DoInteraction()
{
    SharedData::GetInstance()->gamestate = GAME_STATE_SHOP;

    //set cursor
    SharedData::GetInstance()->cursor_xpos = 400;
    SharedData::GetInstance()->cursor_ypos = 400;
    SharedData::GetInstance()->cursor_newxpos = 400;
    SharedData::GetInstance()->cursor_newypos = 400;
}



//flush away~! (get rid of manure)
ManureInteraction::ManureInteraction() : Interaction(true)
{
}

ManureInteraction::~ManureInteraction()
{
}

void ManureInteraction::DoInteraction()
{
    if (SharedData::GetInstance()->gamestate == GAME_STATE_JASIMGAME) {
        SharedData::GetInstance()->floodlevel += 0.1f;

        //randomise interactbutton between 'Q', 'E', 'R', 'T'
        int random = rand() % 7;

        switch (random) {
        case 0:
            SharedData::GetInstance()->interactbutton = 'Q';
            break;
        case 1:
            SharedData::GetInstance()->interactbutton = 'E';
            break;
        case 2:
            SharedData::GetInstance()->interactbutton = 'R';
            break;
        case 3:
            SharedData::GetInstance()->interactbutton = 'T';
            break;
        case 4:
            SharedData::GetInstance()->interactbutton = 'Y';
            break;
        case 5:
            SharedData::GetInstance()->interactbutton = 'F';
            break;
        case 6:
            SharedData::GetInstance()->interactbutton = 'G';
            break;
        }

        if (SharedData::GetInstance()->floodlevel > 3) {
            SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
            SharedData::GetInstance()->player->changeGold(100);
            SharedData::GetInstance()->dialogueProcessor.npc->setLoveMeter(SharedData::GetInstance()->dialogueProcessor.npc->getLoveMeter() + 5);
            SharedData::GetInstance()->shitintoilet = false;
            SharedData::GetInstance()->interactbutton = 'E';
        }
    }
    else {
        SharedData::GetInstance()->canInteract = false;
    }
}

//Lab Game. ChOncHon time
ChonGame::ChonGame() :Interaction(true)
{
}

ChonGame::~ChonGame()
{
}

void ChonGame::DoInteraction()
{
    SharedData::GetInstance()->chonGamebool = true;
}

chonBallInteraction::chonBallInteraction() : Interaction(true)
{
}

chonBallInteraction::~chonBallInteraction()
{
}

void chonBallInteraction::DoInteraction()
{
    SharedData::GetInstance()->ballpickup = true;
}

//MOM'S SPAGHETTI
SpaghettoInteraction::SpaghettoInteraction() : Interaction(true)
{
}

SpaghettoInteraction::~SpaghettoInteraction()
{
}

void SpaghettoInteraction::DoInteraction()
{
	if (SharedData::GetInstance()->player->invfull() == true)
	{
		SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() - 9);
	}
	Interaction* remover;
	remover = new SpaghettoInteraction();
	remover->bound1.Set(9999, 99, 9999); remover->bound2.Set(9999, 99, 9999);
	SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber] = remover;
	SharedData::GetInstance()->player->addItem(4);
}
VeePuzzleSwitchOneInteraction::VeePuzzleSwitchOneInteraction() : Interaction(true)
{
}

VeePuzzleSwitchOneInteraction::~VeePuzzleSwitchOneInteraction()
{
}

void VeePuzzleSwitchOneInteraction::DoInteraction()
{
    SharedData::GetInstance()->one += 1;
    if (SharedData::GetInstance()->one > 6)
        SharedData::GetInstance()->one = 1;
}
VeePuzzleSwitchTwoInteraction::VeePuzzleSwitchTwoInteraction() : Interaction(true)
{
}

VeePuzzleSwitchTwoInteraction::~VeePuzzleSwitchTwoInteraction()
{
}

void VeePuzzleSwitchTwoInteraction::DoInteraction()
{
    SharedData::GetInstance()->two += 1;
    if (SharedData::GetInstance()->two > 6)
        SharedData::GetInstance()->two = 1;
}
VeePuzzleSwitchThreeInteraction::VeePuzzleSwitchThreeInteraction() : Interaction(true)
{
}

VeePuzzleSwitchThreeInteraction::~VeePuzzleSwitchThreeInteraction()
{
}

void VeePuzzleSwitchThreeInteraction::DoInteraction()
{
    SharedData::GetInstance()->three += 1;
    if (SharedData::GetInstance()->three > 6)
        SharedData::GetInstance()->three = 1;
}
VeePuzzleSwitchFourInteraction::VeePuzzleSwitchFourInteraction() : Interaction(true)
{
}

VeePuzzleSwitchFourInteraction::~VeePuzzleSwitchFourInteraction()
{
}

void VeePuzzleSwitchFourInteraction::DoInteraction()
{
    SharedData::GetInstance()->four += 1;
    if (SharedData::GetInstance()->four > 6)
        SharedData::GetInstance()->four = 1;
}
VeePuzzleCheckerSwitchInteraction::VeePuzzleCheckerSwitchInteraction() : Interaction(true)
{
}

VeePuzzleCheckerSwitchInteraction::~VeePuzzleCheckerSwitchInteraction()
{
}

void VeePuzzleCheckerSwitchInteraction::DoInteraction()
{
    if (SharedData::GetInstance()->switchCount < 10)
    {
        std::cout << "test" << std::endl;
        SharedData::GetInstance()->switchFlip = true;
        SharedData::GetInstance()->switchCount += 1;
    }
    //Trigger fail.
}

BedTime::BedTime() : Interaction(true)
{
}

BedTime::~BedTime()
{
}

void BedTime::DoInteraction()
{
	if(SharedData::GetInstance()->player->getHunger() < 100)
	{
		SharedData::GetInstance()->daynighttime += 10;
	}
}

WeedInteraction::WeedInteraction() : Interaction(true)
{
}

WeedInteraction::~WeedInteraction()
{
}

void WeedInteraction::DoInteraction()
{
	if (SharedData::GetInstance()->gamestate == GAME_STATE_WSGAME)
	{
		if (SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber]->bound1.x != 9999)
		{
			Interaction* remover2;
			remover2 = new WeedInteraction();
			remover2->bound1.Set(9999, 99, 9999); remover2->bound2.Set(9999, 99, 9999);
			std::cout << " doInteraction: " << SharedData::GetInstance()->interactnumber << " " << SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber]->bound1 << " " << SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber]->bound2 << std::endl;
			SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber] = remover2;
			SharedData::GetInstance()->player->addItem(3);
			SharedData::GetInstance()->pointscounter += 2;
			SharedData::GetInstance()->weedcounter -= 1;
			if (SharedData::GetInstance()->weedcounter == 0)    //completed game
			{
				SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
				SharedData::GetInstance()->player->changeGold(10 * SharedData::GetInstance()->pointscounter);
                SharedData::GetInstance()->dialogueProcessor.npc->setLoveMeter(SharedData::GetInstance()->dialogueProcessor.npc->getLoveMeter() + 5 + SharedData::GetInstance()->pointscounter);
				SharedData::GetInstance()->pointscounter = 0;
				SharedData::GetInstance()->weedGamebool = false;
			}
		}
	}
	else
	{
		SharedData::GetInstance()->canInteract = false;
	}
}

FarmPlantInteraction::FarmPlantInteraction() : Interaction(true)
{
}

FarmPlantInteraction::~FarmPlantInteraction()
{
}

void FarmPlantInteraction::DoInteraction()
{
	if (SharedData::GetInstance()->gamestate == GAME_STATE_WSGAME)
	{
		if (SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber]->bound1.x != 9999)
		{
			Interaction* remover3;
			remover3 = new FarmPlantInteraction();
			remover3->bound1.Set(9999, 99, 9999); remover3->bound2.Set(9999, 99, 9999);
			std::cout << " doInteraction: " << SharedData::GetInstance()->interactnumber << " " << SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber]->bound1.x << " " << SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber]->bound1.z << std::endl;
			SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber] = remover3;
			SharedData::GetInstance()->player->addItem(2);
			SharedData::GetInstance()->pointscounter -= 1;
		}
	}
	else
	{
		SharedData::GetInstance()->canInteract = false;
	}
}