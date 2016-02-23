#include <string>
#include "Vector3.h"
#include "Item.h"
#include "Outfit.h"

#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
private:
    std::string name_;

protected:
    Character(std::string name, Vector3 position, float direction);

public:
    virtual ~Character();
    std::string getName();
    Vector3 position_;
    float direction_;
};

class Player : public Character
{
private:
    int hunger_;
    unsigned int health_;
    unsigned int gold_;
    //add inventory
    //add pointers for clothes - hats, top, bottoms, etc
    Hat* hat;
    Top* top;
    Bottoms* bottoms;

public:
    Player(std::string name);
    ~Player();
    void Walk(double dt);
    void CheckCollision(float newX, float newZ, bool& zMovement, bool& xMovement);
    void CheckInteraction();
    
    int getHunger();
    unsigned int getHealth();
    unsigned int getGold();

    void setName();

    void setHunger(int hungee);
    void setHealth();
    void setGold();

    bool isDead();

    //movement
    float R_leg_rotation;
    float L_leg_rotation;

    //inventory
    int invselect;
    int inventory[8];
    std::string addItem(int itemID);
    std::string removeItem(int itemPos);
    bool itemHave(int itemID);
};

class NPC : public Character
{
private:
    unsigned int loveMeter_;

public:
    NPC(std::string name) : Character(name, Vector3(0, 0, 0), 0) {}
    ~NPC();
    unsigned int getLoveMeter();
};

#endif