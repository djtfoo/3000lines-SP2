#include <string>
#include "Vector3.h"

#ifndef INTERACTION_H
#define INTERACTION_H

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

//gates opening
class GateInteraction : public Interaction
{
public:
    GateInteraction();
    ~GateInteraction();
    virtual void DoInteraction();
};

//starting dialogue
class Dialogue : public Interaction
{
public:
    Dialogue();
    virtual ~Dialogue();
    virtual void DoInteraction();
};

//entering shop
class ShopInteraction : public Interaction
{
public :
    ShopInteraction();
    ~ShopInteraction();
    virtual void DoInteraction();
};

//Jasim mini-game: toilet
class ManureInteraction : public Interaction
{
public:
    ManureInteraction();
    ~ManureInteraction();
    virtual void DoInteraction();
};

//Chon mini-game: ball movement
class chonBallInteraction : public Interaction
{
public:
    chonBallInteraction();
    ~chonBallInteraction();
    virtual void DoInteraction();
};

//food: randomly spawning spaghetti to be eaten
class SpaghettoInteraction : public Interaction
{
public:
	SpaghettoInteraction();
	~SpaghettoInteraction();
	virtual void DoInteraction();
};

//Vee mini-game: switch 1
class VeePuzzleSwitchOneInteraction : public Interaction
{
public:
    VeePuzzleSwitchOneInteraction();
    ~VeePuzzleSwitchOneInteraction();
    virtual void DoInteraction();
};

//Vee mini-game: switch 2
class VeePuzzleSwitchTwoInteraction : public Interaction
{
public:
    VeePuzzleSwitchTwoInteraction();
    ~VeePuzzleSwitchTwoInteraction();
    virtual void DoInteraction();
};

//Vee mini-game: switch 3
class VeePuzzleSwitchThreeInteraction : public Interaction
{
public:
    VeePuzzleSwitchThreeInteraction();
    ~VeePuzzleSwitchThreeInteraction();
    virtual void DoInteraction();
};

//Vee mini-game: switch 4
class VeePuzzleSwitchFourInteraction : public Interaction
{
public:
    VeePuzzleSwitchFourInteraction();
    ~VeePuzzleSwitchFourInteraction();
    virtual void DoInteraction();
};

//Vee mini-game: checker switch
class VeePuzzleCheckerSwitchInteraction : public Interaction
{
public:
    VeePuzzleCheckerSwitchInteraction();
    ~VeePuzzleCheckerSwitchInteraction();
    virtual void DoInteraction();
};

//Sleeping in bed
class BedTime : public Interaction
{
public:
	BedTime();
	~BedTime();
	virtual void DoInteraction();
};

//Wengstang mini-game: picking up weed
class WeedInteraction : public Interaction
{
public:
	WeedInteraction();
	~WeedInteraction();
	virtual void DoInteraction();
};

//Wengstang mini-game: picking up farm plant
class FarmPlantInteraction : public Interaction
{
public:
	FarmPlantInteraction();
	~FarmPlantInteraction();
	virtual void DoInteraction();
};

#endif