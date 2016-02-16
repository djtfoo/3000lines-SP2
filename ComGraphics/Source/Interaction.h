#include <string>

#ifndef INTERACTION_H
#define INTERACTION_H

class Interaction
{
private:
    std::string name_;  //maybe don't need?

protected:
    Interaction();

public:
    virtual ~Interaction();
    virtual void DoInteraction() = 0;
};

#endif