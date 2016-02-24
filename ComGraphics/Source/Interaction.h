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
    Dialogue() : Interaction() {}
    virtual ~Dialogue() {}
    virtual void DoInteraction() {}
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
#endif