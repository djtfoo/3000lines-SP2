#include <string>

#ifndef INTERACTION_H
#define INTERACTION_H

class Interaction
{
protected:
    Interaction() {}

public:
    Vector3 bound1, bound2;
    virtual ~Interaction() {}
    virtual void DoInteraction() = 0;
};

class PipePuzzleInteraction : public Interaction
{
public:
    PipePuzzleInteraction() : Interaction() {}
    ~PipePuzzleInteraction() {}
    void DoInteraction() {}
};

class Dialogue : public Interaction
{
public:
    Dialogue() : Interaction() {}
    ~Dialogue() {}
    void DoInteraction() {}
};

#endif