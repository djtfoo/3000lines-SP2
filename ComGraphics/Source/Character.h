/******************************************************************************/
/*!
\file	Character.h
\author 3000 Lines Studio
\brief
Character header file
*/
/******************************************************************************/

#include "Vector3.h"
#include "Item.h"
#include "GameState.h"

#include <string>
#include <map>

using std::map;

#ifndef CHARACTER_H
#define CHARACTER_H

/******************************************************************************/
/*!
Class Character:
\brief	Contains information for characters
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
Class Player:
\brief	Contains information for player
*/
/******************************************************************************/
class Player : public Character
{
private:
    int hunger_;
    int health_;
    unsigned int gold_;

public:
    Player();
    Player(std::string name);
    virtual ~Player();
    void Walk(double dt);
    void CheckCollision(float newX, float newZ, bool& zMovement, bool& xMovement);
    void CheckInteraction();
	int footstepsound;

    int getHunger();
    int getHealth();
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

    //Taking Damage
    bool iftakeDamage;
    void takeDamage();
};

/******************************************************************************/
/*!
Class NPC:
\brief	Contains information for Non-playing characters
*/
/******************************************************************************/
class NPC : public Character
{
private:
    int loveMeter_;

public:
    int numberCompliments_;
    bool minigameComplete_;

    NPC(std::string name, const Vector3& pos, std::string textDirectory);
    virtual ~NPC();
    int getLoveMeter();
    void setLoveMeter(const int newLoveMeter);
    map<CONVO_STATE, std::string> dialogues;    //map for dialogues
    std::string Speech();
};

/******************************************************************************/
/*!
Class Enemy:
\brief	Contains information for enemy
*/
/******************************************************************************/
class Enemy : public Character
{
private:
    int e_damage_;
    int e_health_;

public:
    Enemy();
    Enemy(std::string name, const Vector3& pos);
    virtual ~Enemy();

    //Health Set and Get
    unsigned int getHealth();
    void setHealth(int health);

    //State of Enemy
    bool isDead();

    //Take Damage
    bool iftakeDamage;
    void takeDamage(int receiveddamage);

    float yaw;
};

#endif