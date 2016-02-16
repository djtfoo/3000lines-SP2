#include <string>
#include "Vector3.h"

#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
private:
    Character();
    virtual ~Character();
    std::string name_;

public:
    std::string getName();
    Vector3 position_;
    float direction_;
};

class Player : public Character
{
private:
    unsigned int hunger_;
    unsigned int health_;
    unsigned int gold_;
    //add inventory
    //add pointers for clothes - hats, top, bottoms, etc

public:
    Player();
    ~Player();
    void Walk(double dt);
    
    unsigned int getHunger();
    unsigned int getHealth();
    unsigned int getGold();

    unsigned int setHunger();
    unsigned int setHealth();
    unsigned int setGold();

    bool isDead();
};

class NPC : public Character
{
private:
    unsigned int loveMeter_;

public:
    NPC();
    ~NPC();
    unsigned int getLoveMeter();
};

#endif