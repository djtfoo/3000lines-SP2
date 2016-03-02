#include "Vector3.h"
#include "Item.h"
#include "Outfit.h"
#include "GameState.h"

#include <string>
#include <map>

using std::map;

#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
private:
    std::string name_;

protected:
    Character(std::string name, Vector3 position, float 
        
        );

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
    //add pointers for clothes - hats, top, bottoms, etc
    Hat* hat;
    Top* top;
    Bottoms* bottoms;

public:
    Player(std::string name);
    virtual ~Player();
    void Walk(double dt);
    void CheckCollision(float newX, float newZ, bool& zMovement, bool& xMovement);
    void CheckInteraction();

    int getHunger();
    unsigned int getHealth();
    unsigned int getGold();

    void setName();

    void setHunger(int hungee);
    void setHealth();
    void setGold(int moneh);
    void changeGold(int change);

    bool isDead();

    //movement
    float R_leg_rotation;
    float L_leg_rotation;

    //inventory
    int invselect;
    int inventory[8];
    int addItem(int itemID);
    int removeItem(int itemPos);
	void useItem(int itemID);
	void convertItem(int itemID1, int itemID2);
    int itemHave(int itemID);
	bool invfull();
	bool invfulldisplay;
};

class NPC : public Character
{
private:
    int loveMeter_;

public:
    NPC(std::string name, const Vector3& pos, std::string textDirectory);
    virtual ~NPC();
    int getLoveMeter();
    void setLoveMeter(const int newLoveMeter);
    map<CONVO_STATE, std::string> dialogues;    //map for dialogues
    std::string Speech();
};

#endif