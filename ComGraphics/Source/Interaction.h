/******************************************************************************/
/*!
\file	Interaction.h
\author 3000 Lines Studio
\brief
Interaction header file
*/
/******************************************************************************/

#include <string>
#include "Vector3.h"

#ifndef INTERACTION_H
#define INTERACTION_H


/******************************************************************************/
/*!
Class Interaction:
\brief	Contains the information to check interaction
*/
/******************************************************************************/
//interface class
class Interaction
{
protected:
    Interaction(bool pressbutton);

public:
    Vector3 bound1, bound2;
    virtual ~Interaction();
    const bool pressButton;     //whether this interaction requires a key press
    virtual void DoInteraction() = 0;   //simple do-interactions

};

/******************************************************************************/
/*!
Class GateInteraction:
\brief	Contains the interaction for gates
*/
/******************************************************************************/
//gates opening
class GateInteraction : public Interaction
{
public:
    GateInteraction();
    ~GateInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class Dialogue:
\brief	Contains the interaction for dialogue
*/
/******************************************************************************/
//starting dialogue
class Dialogue : public Interaction
{
public:
    Dialogue();
    virtual ~Dialogue();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class ShopInteraction:
\brief	Contains the interaction for shop
*/
/******************************************************************************/
//entering shop
class ShopInteraction : public Interaction
{
public :
    ShopInteraction();
    ~ShopInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class ManureInteraction:
\brief	Contains the interaction for toilet minigame
*/
/******************************************************************************/
//Jasim mini-game: toilet
class ManureInteraction : public Interaction
{
public:
    ManureInteraction();
    ~ManureInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class chonBallInteraction:
\brief	Contains the interaction for lab ball minigame
*/
/******************************************************************************/
//Chon mini-game: ball movement
class chonBallInteraction : public Interaction
{
public:
    chonBallInteraction();
    ~chonBallInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class SpaghettoInteraction:
\brief	Contains the interaction for spaghetti
*/
/******************************************************************************/
//food: randomly spawning spaghetti to be eaten
class SpaghettoInteraction : public Interaction
{
public:
	SpaghettoInteraction();
	~SpaghettoInteraction();
	virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class VeePuzzleSwitchOneInteraction:
\brief	Contains the interaction for first puzzle switch
*/
/******************************************************************************/
//Vee mini-game: switch 1
class VeePuzzleSwitchOneInteraction : public Interaction
{
public:
    VeePuzzleSwitchOneInteraction();
    ~VeePuzzleSwitchOneInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class VeePuzzleSwitchTwoInteraction:
\brief	Contains the interaction for second puzzle switch
*/
/******************************************************************************/
//Vee mini-game: switch 2
class VeePuzzleSwitchTwoInteraction : public Interaction
{
public:
    VeePuzzleSwitchTwoInteraction();
    ~VeePuzzleSwitchTwoInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class VeePuzzleSwitchThreeInteraction:
\brief	Contains the interaction for third puzzle switch
*/
/******************************************************************************/
//Vee mini-game: switch 3
class VeePuzzleSwitchThreeInteraction : public Interaction
{
public:
    VeePuzzleSwitchThreeInteraction();
    ~VeePuzzleSwitchThreeInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class VeePuzzleSwitchFourInteraction:
\brief	Contains the interaction for fourth puzzle switch
*/
/******************************************************************************/
//Vee mini-game: switch 4
class VeePuzzleSwitchFourInteraction : public Interaction
{
public:
    VeePuzzleSwitchFourInteraction();
    ~VeePuzzleSwitchFourInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class VeePuzzleCheckerSwitchInteraction:
\brief	Contains the interaction for puzzle checker switch
*/
/******************************************************************************/
//Vee mini-game: checker switch
class VeePuzzleCheckerSwitchInteraction : public Interaction
{
public:
    VeePuzzleCheckerSwitchInteraction();
    ~VeePuzzleCheckerSwitchInteraction();
    virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class BedTime:
\brief	Contains the interaction for bed
*/
/******************************************************************************/
//Sleeping in bed
class BedTime : public Interaction
{
public:
	BedTime();
	~BedTime();
	virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class WeedInteraction:
\brief	Contains the interaction for picking weed in weed minigame
*/
/******************************************************************************/
//Wengstang mini-game: picking up weed
class WeedInteraction : public Interaction
{
public:
	WeedInteraction();
	~WeedInteraction();
	virtual void DoInteraction();
};

/******************************************************************************/
/*!
Class FarmPlantInteraction:
\brief	Contains the interaction for picking farm plant in weed minigame
*/
/******************************************************************************/
//Wengstang mini-game: picking up farm plant
class FarmPlantInteraction : public Interaction
{
public:
	FarmPlantInteraction();
	~FarmPlantInteraction();
	virtual void DoInteraction();
};

#endif