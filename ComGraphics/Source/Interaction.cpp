#include "Interaction.h"

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