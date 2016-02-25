#include <string>
#include "Vector3.h"

#ifndef INTERACTION_H
#define INTERACTION_H

class Interaction
{
protected:
    Interaction();

public:
    Vector3 bound1, bound2;
    virtual ~Interaction();
    virtual void DoInteraction() = 0;   //simple do-interactions
};

class GateInteraction : public Interaction
{
public:
    GateInteraction();
    ~GateInteraction();
    virtual void DoInteraction();
};

class PipePuzzleInteraction : public Interaction
{
public:
    PipePuzzleInteraction();
    virtual ~PipePuzzleInteraction();
    virtual void DoInteraction();
};

class Dialogue : public Interaction
{
public:
    Dialogue();
    virtual ~Dialogue();
    virtual void DoInteraction();
};

class ShopInteraction : public Interaction
{
public :
    ShopInteraction();
    ~ShopInteraction();
    virtual void DoInteraction();
};

class ManureInteraction : public Interaction
{
public:
    ManureInteraction();
    ~ManureInteraction();
    virtual void DoInteraction();
};

class ChonGame : public Interaction
{
public:
    ChonGame();
    ~ChonGame();
    virtual void DoInteraction();
};

class chonBallInteraction : public Interaction
{
public:
    chonBallInteraction();
    ~chonBallInteraction();
    virtual void DoInteraction();
};


class SpaghettoInteraction : public Interaction
{
public:
	SpaghettoInteraction();
	~SpaghettoInteraction();
	virtual void DoInteraction();
};
class VeePuzzleSwitchOneInteraction : public Interaction
{
public:
    VeePuzzleSwitchOneInteraction();
    ~VeePuzzleSwitchOneInteraction();
    virtual void DoInteraction();
};
class VeePuzzleSwitchTwoInteraction : public Interaction
{
public:
    VeePuzzleSwitchTwoInteraction();
    ~VeePuzzleSwitchTwoInteraction();
    virtual void DoInteraction();
};
class VeePuzzleSwitchThreeInteraction : public Interaction
{
public:
    VeePuzzleSwitchThreeInteraction();
    ~VeePuzzleSwitchThreeInteraction();
    virtual void DoInteraction();
};
class VeePuzzleSwitchFourInteraction : public Interaction
{
public:
    VeePuzzleSwitchFourInteraction();
    ~VeePuzzleSwitchFourInteraction();
    virtual void DoInteraction();
};
class VeePuzzleCheckerSwitchInteraction : public Interaction
{
public:
    VeePuzzleCheckerSwitchInteraction();
    ~VeePuzzleCheckerSwitchInteraction();
    virtual void DoInteraction();
};

class BedTime : public Interaction
{
public:
	BedTime();
	~BedTime();
	virtual void DoInteraction();
};
#endif