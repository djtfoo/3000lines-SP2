/******************************************************************************/
/*!
\file	Interaction.cpp
\author 3000 Lines Studio
\brief
Functions to check interactions
*/
/******************************************************************************/

#include "Interaction.h"
#include "SharedData.h"

//abstract class of interaction
/******************************************************************************/
/*!
\brief	Interaction default constructor
\param pressbutton
check if button is pressed
*/
/******************************************************************************/
Interaction::Interaction(bool pressbutton) : pressButton(pressbutton)
{
}

/******************************************************************************/
/*!
\brief	Interaction default destructor
*/
/******************************************************************************/
Interaction::~Interaction()
{
}

//Gate interaction open/close
/******************************************************************************/
/*!
\brief	GateInteraction default constructor
*/
/******************************************************************************/
GateInteraction::GateInteraction() : Interaction(false)
{
}

/******************************************************************************/
/*!
\brief	GateInteraction default destructor
*/
/******************************************************************************/
GateInteraction::~GateInteraction()
{
}
/******************************************************************************/
/*!
\brief	GateInteraction DoInteraction
*/
/******************************************************************************/
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

//Dialogue Interaction class
/******************************************************************************/
/*!
\brief	Dialogue default constructor
*/
/******************************************************************************/
Dialogue::Dialogue() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	Dialogue default destructor
*/
/******************************************************************************/
Dialogue::~Dialogue()
{
}
/******************************************************************************/
/*!
\brief	Dialogue DoInteraction
*/
/******************************************************************************/
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

                if (SharedData::GetInstance()->daynumber <= 7) {
                    SharedData::GetInstance()->dialogueProcessor.convostate = CONVO_INTRO;
                }
                else {
                    SharedData::GetInstance()->dialogueProcessor.convostate = CONVO_BOSSPROMPT;
                }
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
/******************************************************************************/
/*!
\brief	ShopInteraction default constructor
*/
/******************************************************************************/
ShopInteraction::ShopInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	ShopInteraction default destructor
*/
/******************************************************************************/
ShopInteraction::~ShopInteraction()
{
}
/******************************************************************************/
/*!
\brief	ShopInteraction DoInteraction
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\brief	ManureInteraction default constructor
*/
/******************************************************************************/
ManureInteraction::ManureInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	ManureInteraction default destructor
*/
/******************************************************************************/
ManureInteraction::~ManureInteraction()
{
}
/******************************************************************************/
/*!
\brief	ManureInteraction DoInteraction
*/
/******************************************************************************/
void ManureInteraction::DoInteraction()
{
    if (SharedData::GetInstance()->gamestate == GAME_STATE_JASIMGAME) {
        SharedData::GetInstance()->floodlevel += 0.1f;

        //randomise interactbutton
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
            SharedData::GetInstance()->gamestate = GAME_STATE_DIALOGUE;
            SharedData::GetInstance()->dialogueProcessor.convostate = CONVO_FINISHMINIGAME;
            
            //gain gold and love meter based on timer
            int gained = 50 - (int)(SharedData::GetInstance()->timeElapsed);
            if (gained < 0) {
                gained = 0;
            }
            SharedData::GetInstance()->player->changeGold(4 * gained);
            SharedData::GetInstance()->dialogueProcessor.npc->setLoveMeter(SharedData::GetInstance()->dialogueProcessor.npc->getLoveMeter() + (gained / 3));

            //reset mini-game
            SharedData::GetInstance()->timeElapsed = 0;
            SharedData::GetInstance()->shitintoilet = false;
            SharedData::GetInstance()->interactbutton = 'E';    //change interactbutton back to 'E'
        }
    }
    else {
        SharedData::GetInstance()->canInteract = false;
    }
}

/******************************************************************************/
/*!
\brief	chonBallInteraction default constructor
*/
/******************************************************************************/
chonBallInteraction::chonBallInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	chonBallInteraction default destructor
*/
/******************************************************************************/
chonBallInteraction::~chonBallInteraction()
{
}
/******************************************************************************/
/*!
\brief	chonBallInteraction DoInteraction
*/
/******************************************************************************/
void chonBallInteraction::DoInteraction()
{
    SharedData::GetInstance()->ballpickup = true;
}

//MOM'S SPAGHETTI
/******************************************************************************/
/*!
\brief	SpaghettoInteraction default constructor
*/
/******************************************************************************/
SpaghettoInteraction::SpaghettoInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	SpaghettoInteraction default destructor
*/
/******************************************************************************/
SpaghettoInteraction::~SpaghettoInteraction()
{
}
/******************************************************************************/
/*!
\brief	SpaghettoInteraction DoInteraction
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
\brief	VeePuzzleSwitchOneInteraction default constructor
*/
/******************************************************************************/
VeePuzzleSwitchOneInteraction::VeePuzzleSwitchOneInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	VeePuzzleSwitchOneInteraction default destructor
*/
/******************************************************************************/
VeePuzzleSwitchOneInteraction::~VeePuzzleSwitchOneInteraction()
{
}
/******************************************************************************/
/*!
\brief	VeePuzzleSwitchOneInteraction DoInteraction
*/
/******************************************************************************/
void VeePuzzleSwitchOneInteraction::DoInteraction()
{
    SharedData::GetInstance()->one += 1;
    if (SharedData::GetInstance()->one > 6)
        SharedData::GetInstance()->one = 1;
}

/******************************************************************************/
/*!
\brief	VeePuzzleSwitchTwoInteraction default constructor
*/
/******************************************************************************/
VeePuzzleSwitchTwoInteraction::VeePuzzleSwitchTwoInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	VeePuzzleSwitchTwoInteraction default destructor
*/
/******************************************************************************/
VeePuzzleSwitchTwoInteraction::~VeePuzzleSwitchTwoInteraction()
{
}
/******************************************************************************/
/*!
\brief	VeePuzzleSwitchTwoInteraction DoInteraction
*/
/******************************************************************************/
void VeePuzzleSwitchTwoInteraction::DoInteraction()
{
    SharedData::GetInstance()->two += 1;
    if (SharedData::GetInstance()->two > 6)
        SharedData::GetInstance()->two = 1;
}

/******************************************************************************/
/*!
\brief	VeePuzzleSwitchThreeInteraction default constructor
*/
/******************************************************************************/
VeePuzzleSwitchThreeInteraction::VeePuzzleSwitchThreeInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	VeePuzzleSwitchThreeInteraction default destructor
*/
/******************************************************************************/
VeePuzzleSwitchThreeInteraction::~VeePuzzleSwitchThreeInteraction()
{
}
/******************************************************************************/
/*!
\brief	VeePuzzleSwitchThreeInteraction DoInteraction
*/
/******************************************************************************/
void VeePuzzleSwitchThreeInteraction::DoInteraction()
{
    SharedData::GetInstance()->three += 1;
    if (SharedData::GetInstance()->three > 6)
        SharedData::GetInstance()->three = 1;
}

/******************************************************************************/
/*!
\brief	VeePuzzleSwitchFourInteraction default constructor
*/
/******************************************************************************/
VeePuzzleSwitchFourInteraction::VeePuzzleSwitchFourInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	VeePuzzleSwitchFourInteraction default destructor
*/
/******************************************************************************/
VeePuzzleSwitchFourInteraction::~VeePuzzleSwitchFourInteraction()
{
}
/******************************************************************************/
/*!
\brief	VeePuzzleSwitchFourInteraction DoInteraction
*/
/******************************************************************************/
void VeePuzzleSwitchFourInteraction::DoInteraction()
{
    SharedData::GetInstance()->four += 1;
    if (SharedData::GetInstance()->four > 6)
        SharedData::GetInstance()->four = 1;
}

/******************************************************************************/
/*!
\brief	VeePuzzleCheckerSwitchInteraction default constructor
*/
/******************************************************************************/
VeePuzzleCheckerSwitchInteraction::VeePuzzleCheckerSwitchInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	VeePuzzleCheckerSwitchInteraction default destructor
*/
/******************************************************************************/
VeePuzzleCheckerSwitchInteraction::~VeePuzzleCheckerSwitchInteraction()
{
}
/******************************************************************************/
/*!
\brief	VeePuzzleCheckerSwitchInteraction DoInteraction
*/
/******************************************************************************/
void VeePuzzleCheckerSwitchInteraction::DoInteraction()
{
    SharedData::GetInstance()->switchFlip = true;
    SharedData::GetInstance()->switchCount += 1;
}

/******************************************************************************/
/*!
\brief	BedTime default constructor
*/
/******************************************************************************/
BedTime::BedTime() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	BedTime default destructor
*/
/******************************************************************************/
BedTime::~BedTime()
{
}
/******************************************************************************/
/*!
\brief	BedTime DoInteraction
*/
/******************************************************************************/
void BedTime::DoInteraction()
{
	if(SharedData::GetInstance()->player->getHunger() < 100)
	{
        SharedData::GetInstance()->sleep = true;
	}
}

/******************************************************************************/
/*!
\brief	WeedInteraction default constructor
*/
/******************************************************************************/
WeedInteraction::WeedInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	WeedInteraction default destructor
*/
/******************************************************************************/
WeedInteraction::~WeedInteraction()
{
}
/******************************************************************************/
/*!
\brief	WeedInteraction DoInteraction
*/
/******************************************************************************/
void WeedInteraction::DoInteraction()
{
	if (SharedData::GetInstance()->gamestate == GAME_STATE_WSGAME)
	{
		if (SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber]->bound1.x != 9999)
		{
			Interaction* remover2;
			remover2 = new WeedInteraction();
			remover2->bound1.Set(9999, 99, 9999); remover2->bound2.Set(9999, 99, 9999);
			SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber] = remover2;
			SharedData::GetInstance()->player->addItem(3);
			SharedData::GetInstance()->pointscounter += 2;
			SharedData::GetInstance()->weedcounter -= 1;
			if (SharedData::GetInstance()->weedcounter == 0)    //completed game
			{
                SharedData::GetInstance()->gamestate = GAME_STATE_DIALOGUE;
                SharedData::GetInstance()->dialogueProcessor.convostate = CONVO_FINISHMINIGAME;
				SharedData::GetInstance()->player->changeGold(10 * SharedData::GetInstance()->pointscounter);
                SharedData::GetInstance()->dialogueProcessor.npc->setLoveMeter(SharedData::GetInstance()->dialogueProcessor.npc->getLoveMeter() + SharedData::GetInstance()->pointscounter / 2);
				SharedData::GetInstance()->pointscounter = 0;
			}
		}
	}
	else
	{
		SharedData::GetInstance()->canInteract = false;
	}
}

/******************************************************************************/
/*!
\brief	FarmPlantInteraction default constructor
*/
/******************************************************************************/
FarmPlantInteraction::FarmPlantInteraction() : Interaction(true)
{
}

/******************************************************************************/
/*!
\brief	FarmPlantInteraction default destructor
*/
/******************************************************************************/
FarmPlantInteraction::~FarmPlantInteraction()
{
}
/******************************************************************************/
/*!
\brief	FarmPlantInteraction DoInteraction
*/
/******************************************************************************/
void FarmPlantInteraction::DoInteraction()
{
	if (SharedData::GetInstance()->gamestate == GAME_STATE_WSGAME)
	{
		if (SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber]->bound1.x != 9999)
		{
			Interaction* remover3;
			remover3 = new FarmPlantInteraction();
			remover3->bound1.Set(9999, 99, 9999); remover3->bound2.Set(9999, 99, 9999);
			SharedData::GetInstance()->interactionItems[SharedData::GetInstance()->interactnumber] = remover3;
			//SharedData::GetInstance()->player->addItem(2);
			SharedData::GetInstance()->pointscounter -= 1;
		}
	}
	else
	{
		SharedData::GetInstance()->canInteract = false;
	}
}