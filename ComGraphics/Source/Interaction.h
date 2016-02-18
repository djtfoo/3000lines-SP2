#include <string>

#ifndef INTERACTION_H
#define INTERACTION_H

class Interaction
{
protected:
    Interaction() {}

public:
    Vector3 middlePoint;
    float distX, distY, distZ;
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

#endif