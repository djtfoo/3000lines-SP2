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
VeePuzzleInteraction::VeePuzzleInteraction() : Interaction()
{
}

VeePuzzleInteraction::~VeePuzzleInteraction()
{
}

void VeePuzzleInteraction::DoInteraction()
{
}