#include "Interaction.h"
#include "SharedData.h"

//abstract class of interaction
Interaction::Interaction()
{
}

Interaction::~Interaction()
{
}

//pipe puzzle interaction (rotation)
PipePuzzleInteraction::PipePuzzleInteraction() : Interaction()
{
}

PipePuzzleInteraction::~PipePuzzleInteraction()
{
}

void PipePuzzleInteraction::DoInteraction()
{
}

//Dialogue Interaction class
Dialogue::Dialogue() : Interaction()
{
}

Dialogue::~Dialogue()
{
}

void Dialogue::DoInteraction()
{
    for (vector<NPC*>::iterator it = SharedData::GetInstance()->NPCs.begin(); it != SharedData::GetInstance()->NPCs.end(); ++it) {
        Vector3 b1 = SharedData::GetInstance()->interactptr->bound1;
        Vector3 b2 = SharedData::GetInstance()->interactptr->bound2;
        if ((*it)->position_.x >= b1.x && (*it)->position_.x <= b2.x &&
            (*it)->position_.y >= b1.y && (*it)->position_.y <= b2.y &&
            (*it)->position_.z >= b1.z && (*it)->position_.z <= b2.z) {
            SharedData::GetInstance()->dialogueProcessor.npc = *it;
            SharedData::GetInstance()->gamestate = GAME_STATE_DIALOGUE;
            break;
        }
    }
}

//shop interaction (enter shop state)
ShopInteraction::ShopInteraction() : Interaction()
{
}

ShopInteraction::~ShopInteraction()
{
}

void ShopInteraction::DoInteraction()
{
    SharedData::GetInstance()->gamestate = GAME_STATE_SHOP;
}



//flush away~! (get rid of manure)
ManureInteraction::ManureInteraction() : Interaction()
{
}

ManureInteraction::~ManureInteraction()
{
}

void ManureInteraction::DoInteraction()
{
	SharedData::GetInstance()->toiletflood = true;
}

//Lab Game. ChOncHon time
ChonGame::ChonGame() :Interaction()
{
}

ChonGame::~ChonGame()
{
}

void ChonGame::DoInteraction()
{
    SharedData::GetInstance()->chonGamebool = true;
}

chonBallInteraction::chonBallInteraction() : Interaction()
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
SpaghettoInteraction::SpaghettoInteraction() : Interaction()
{
}

SpaghettoInteraction::~SpaghettoInteraction()
{
}

void SpaghettoInteraction::DoInteraction()
{
	SharedData::GetInstance()->toiletflood = true;
}
VeePuzzleSwitchOneInteraction::VeePuzzleSwitchOneInteraction() : Interaction()
{
}

VeePuzzleSwitchOneInteraction::~VeePuzzleSwitchOneInteraction()
{
}

void VeePuzzleSwitchOneInteraction::DoInteraction()
{
    SharedData::GetInstance()->one += 1;
    if (SharedData::GetInstance()->one > 5)
        SharedData::GetInstance()->one = 1;
}
VeePuzzleSwitchTwoInteraction::VeePuzzleSwitchTwoInteraction() : Interaction()
{
}

VeePuzzleSwitchTwoInteraction::~VeePuzzleSwitchTwoInteraction()
{
}

void VeePuzzleSwitchTwoInteraction::DoInteraction()
{
    SharedData::GetInstance()->two += 1;
    if (SharedData::GetInstance()->two > 5)
        SharedData::GetInstance()->two = 1;
}
VeePuzzleSwitchThreeInteraction::VeePuzzleSwitchThreeInteraction() : Interaction()
{
}

VeePuzzleSwitchThreeInteraction::~VeePuzzleSwitchThreeInteraction()
{
}

void VeePuzzleSwitchThreeInteraction::DoInteraction()
{
    SharedData::GetInstance()->three += 1;
    if (SharedData::GetInstance()->three > 5)
        SharedData::GetInstance()->three = 1;
}
VeePuzzleSwitchFourInteraction::VeePuzzleSwitchFourInteraction() : Interaction()
{
}

VeePuzzleSwitchFourInteraction::~VeePuzzleSwitchFourInteraction()
{
}

void VeePuzzleSwitchFourInteraction::DoInteraction()
{
    SharedData::GetInstance()->four += 1;
    if (SharedData::GetInstance()->four > 5)
        SharedData::GetInstance()->four = 1;
}
VeePuzzleCheckerSwitchInteraction::VeePuzzleCheckerSwitchInteraction() : Interaction()
{
}

VeePuzzleCheckerSwitchInteraction::~VeePuzzleCheckerSwitchInteraction()
{
}

void VeePuzzleCheckerSwitchInteraction::DoInteraction()
{
    SharedData::GetInstance()->switchFlip = true;
}

BedTime::BedTime() : Interaction()
{
}

BedTime::~BedTime()
{
}

void BedTime::DoInteraction()
{
}