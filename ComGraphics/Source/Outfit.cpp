#include "Outfit.h"

//Outfit abstract class
Outfit::Outfit() : Item()
{
}

Outfit::Outfit(std::string name) : Item(name)
{
}

Outfit::~Outfit()
{
}

//Hat outfit
Hat::Hat() : Outfit()
{
}

Hat::Hat(std::string name) : Outfit(name)
{
}

Hat::~Hat()
{
}

//Top outfit
Top::Top() : Outfit()
{
}

Top::Top(std::string name) : Outfit(name)
{
}

Top::~Top()
{
}

//Bottoms outfit
Bottoms::Bottoms() : Outfit()
{
}

Bottoms::Bottoms(std::string name) : Outfit(name)
{
}

Bottoms::~Bottoms()
{
}