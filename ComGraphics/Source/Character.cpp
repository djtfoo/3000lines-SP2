/******************************************************************************/
/*!
\file	Character.cpp
\author 3000 Lines Studio
\brief
Functions for the character and inventory
*/
/******************************************************************************/

#include "Character.h"

#include "Application.h"
#include "SharedData.h"

#include <sstream>
#include <fstream>

/******************************************************************************/
/*!
\brief	Character default constructor
*/
/******************************************************************************/
Character::Character(std::string name, Vector3 position, float 
    
    ) : name_(name), position_(position), direction_(0)
{
}

/******************************************************************************/
/*!
\brief	Character default destructor
*/
/******************************************************************************/
Character::~Character()
{
}
/******************************************************************************/
/*!
\brief	Getting Name of Character

\return
String of the Name of character.
*/
/******************************************************************************/
std::string Character::getName()
{
    return name_;
}

/******************************************************************************/
/*!
\brief	Player default constructor
*/
/******************************************************************************/

Player::Player() : Character("", Vector3(0, 25, 0), 0), hunger_(0), health_(100), gold_(500), invselect(0), invfulldisplay(false), iftakeDamage(false), footstepsound(0)
{
}

/******************************************************************************/
/*!
\brief	Player default constructor
\param name
name of player
*/
/******************************************************************************/

Player::Player(std::string name) : Character(name, Vector3(0, 25, 0), 0), hunger_(0), health_(100), gold_(500), invselect(0), invfulldisplay(false), iftakeDamage(false), footstepsound(0)
{
	for (int i = 0; i < 8; i++)
	{
		inventory[i] = 0;
	}
}

/******************************************************************************/
/*!
\brief	Player default destructor
*/
/******************************************************************************/
Player::~Player()
{
}

void Player::setHealth()
{
    health_ = 100;
}

static float WALKSPEED = 40.f;
/******************************************************************************/
/*!
\brief	Walk and Movement of Player.

\param dt
    Value of elapsed time.
\return
    None
*/
/******************************************************************************/
void Player::Walk(double dt)
{
    bool xMovement = true, zMovement = true;
    float newX = position_.x;
    float newZ = position_.z;

	if ((Application::IsKeyPressed(VK_SHIFT)) && (SharedData::GetInstance()->player->getHunger() < 100)) {   //sped up moving
        WALKSPEED = 130.f;
    }
    else {
        WALKSPEED = 40.f;
    }
    
    if (Application::IsKeyPressed('W')) {   //player moves forward
		footstepsound = 35;
		if ((Application::IsKeyPressed(VK_SHIFT)) && (SharedData::GetInstance()->player->getHunger() < 100))
		{
			footstepsound = 16;
		}
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position);
        Vector3 view2 = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        view.y = 0;
        view = view.Normalized();

        newX = (float)(SharedData::GetInstance()->camera->target.x + view.x * dt * WALKSPEED);
        newZ = (float)(SharedData::GetInstance()->camera->target.z + view.z * dt * WALKSPEED);

        CheckCollision(newX, newZ, xMovement, zMovement);

        if (xMovement) {
            position_.x = newX;
            SharedData::GetInstance()->camera->target.x = newX;
            //SharedData::GetInstance()->camera->position.x = newX - view.x;
        }
        if (zMovement) {
            position_.z = newZ;
            SharedData::GetInstance()->camera->target.z = newZ;
            //SharedData::GetInstance()->camera->position.z = newZ - view.z;
        }
        SharedData::GetInstance()->camera->position = SharedData::GetInstance()->camera->target - view2;
    }
	else
	{
		footstepsound = 0;
	}

    if (Application::IsKeyPressed('S')) {   //player moves backward
		footstepsound = 35;
		if ((Application::IsKeyPressed(VK_SHIFT)) && (SharedData::GetInstance()->player->getHunger() < 100))
		{
			footstepsound = 16;
		}
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position);
        Vector3 view2 = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        view.y = 0;
        view = view.Normalized();

        newX = (float)(SharedData::GetInstance()->camera->target.x - view.x * dt * WALKSPEED);
        newZ = (float)(SharedData::GetInstance()->camera->target.z - view.z * dt * WALKSPEED);

        CheckCollision(newX, newZ, xMovement, zMovement);

        if (xMovement) {
            position_.x = newX;
            SharedData::GetInstance()->camera->target.x = newX;
            //SharedData::GetInstance()->camera->position.x = newX - view.x;
        }
        if (zMovement) {
            position_.z = newZ;
            SharedData::GetInstance()->camera->target.z = newZ;
            //SharedData::GetInstance()->camera->position.z = newZ - view.z;
        }
        SharedData::GetInstance()->camera->position = SharedData::GetInstance()->camera->target - view2;
    }

    if (Application::IsKeyPressed('A')) {   //strafe left
		footstepsound = 35;
		if ((Application::IsKeyPressed(VK_SHIFT)) && (SharedData::GetInstance()->player->getHunger() < 100))
		{
			footstepsound = 16;
		}
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        Vector3 right = view.Cross(SharedData::GetInstance()->camera->up);
        right.y = 0;
        right.Normalize();

        newX = (float)(SharedData::GetInstance()->camera->target.x - right.x * dt * WALKSPEED);
        newZ = (float)(SharedData::GetInstance()->camera->target.z - right.z * dt * WALKSPEED);

        CheckCollision(newX, newZ, xMovement, zMovement);

        if (xMovement) {
            position_.x = newX;
            SharedData::GetInstance()->camera->target.x = newX;
            //SharedData::GetInstance()->camera->position.x = newX - view.x;
        }
        if (zMovement) {
            position_.z = newZ;
            SharedData::GetInstance()->camera->target.z = newZ;
            //SharedData::GetInstance()->camera->position.z = newZ - view.z;
        }
        SharedData::GetInstance()->camera->position = SharedData::GetInstance()->camera->target - view;
    }

    if (Application::IsKeyPressed('D')) {   //strafe right
		footstepsound = 35;
		if ((Application::IsKeyPressed(VK_SHIFT)) && (SharedData::GetInstance()->player->getHunger() < 100))
		{
			footstepsound = 16;
		}
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        Vector3 right = view.Cross(SharedData::GetInstance()->camera->up);
        right.y = 0;
        right.Normalize();

        newX = (float)(SharedData::GetInstance()->camera->target.x + right.x * dt * WALKSPEED);
        newZ = (float)(SharedData::GetInstance()->camera->target.z + right.z * dt * WALKSPEED);

        CheckCollision(newX, newZ, xMovement, zMovement);

        if (xMovement) {
            position_.x = newX;
            SharedData::GetInstance()->camera->target.x = newX;
            //SharedData::GetInstance()->camera->position.x = newX - view.x;
        }
        if (zMovement) {
            position_.z = newZ;
            SharedData::GetInstance()->camera->target.z = newZ;
            //SharedData::GetInstance()->camera->position.z = newZ - view.z;
        }
        SharedData::GetInstance()->camera->position = SharedData::GetInstance()->camera->target - view;
    }

}

/******************************************************************************/
/*!
\brief	Checking Player Collision with everything

\param newX
    Value containing the X value of the player.
\param newZ
    Value containing the Z value of the player.
\param xMovement
    Bool to check whether a player can move in the X direction.
\param zMovement
    Bool to check whether a player can move in the Z direction.
\return
    None
*/
/******************************************************************************/
void Player::CheckCollision(float newX, float newZ, bool& xMovement, bool& zMovement)
{
    map<LOCATION, vector<ItemCollision>>::iterator it = SharedData::GetInstance()->collisionMap.find(SharedData::GetInstance()->location);
    vector<ItemCollision> temp = it->second;

    for (vector<ItemCollision>::iterator i = temp.begin(); i != temp.end(); ++i) {
        if (position_.x > i->minX && position_.x < i->maxX) {
            if (newZ > i->minZ && newZ < i->maxZ) {
                zMovement = false;
            }
        }
        else if (position_.z > i->minZ && position_.z < i->maxZ) {
            if (newX > i->minX && newX < i->maxX) {
                xMovement = false;
            }
        }
    }

}
/******************************************************************************/
/*!
\brief	Check if anything Intersects with the Player

\param dist1
    Value of the max view the player could have in a direction
\param dist2
    Value of the players max Position could have in a direction
\param maxView
    Value which determines the max view of the player
\param playerPos
    Players positon in 3D space
\param intersect
    Contains Vector that holds intersection vector of objects
\return
    True if Player doesn't have equal view and pos or if either one is 0.
*/
/******************************************************************************/
bool inline GetIntersection(float dist1, float dist2, Vector3 maxView, Vector3 playerPos, Vector3& intersect)
{
    if ((dist1 * dist2) >= 0.0f)
        return false;
    if (dist1 == dist2)
        return false;
    
    intersect = maxView + (playerPos - maxView) * (-dist1 / (dist2 - dist1));
    return true;
}
/******************************************************************************/
/*!
\brief	Checking Hitbox.

\param intersect
    Contains Vector to check if player intersects with hitbox.
\param bound1
    Vector that contains the first point of a Hit box.
\param bound2
    Vector that contains teh second point of a Hit box.
\param axis
    Value that contains which axis is going to be used on (x, y, z)
\return TRUE
    When any of the intersect Vectors cross the bounds, triggering inbox to be true.
*/
/******************************************************************************/
bool inline InBox(Vector3 intersect, Vector3 bound1, Vector3 bound2, const int axis)
{
    bool inbox = false;
    switch (axis)
    {
    case 1:     //x-axis
        if (intersect.y > bound1.y && intersect.y < bound2.y &&
            intersect.z > bound1.z && intersect.z < bound2.z) {
            inbox = true;
        }
        break;
    case 2:     //y-axis
        if (intersect.x > bound1.x && intersect.x < bound2.x &&
            intersect.z > bound1.z && intersect.z < bound2.z) {
            inbox = true;
        }
        break;
    case 3:     //z-axis
        if (intersect.x > bound1.x && intersect.x < bound2.x &&
            intersect.y > bound1.y && intersect.y < bound2.y) {
            inbox = true;
        }
        break;
    }

    return inbox;
}
/******************************************************************************/
/*!
\brief	Checking Player Interaction
    Function to Check if player has interacted with any object.
*/
/******************************************************************************/
void Player::CheckInteraction()
{
    SharedData::GetInstance()->canInteract = false;
    SharedData::GetInstance()->gatelookat = false;

    vector<Interaction*> temp = SharedData::GetInstance()->interactionItems;
    Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
    Vector3 maxView = position_ + 50 * view;
	int i = 0;
    for (vector<Interaction*>::iterator it = temp.begin(); it != temp.end(); ++it) {
		SharedData::GetInstance()->interactnumber = i;
        //if the view is totally outside the box, then don't do calculations because confirm no interaction
		if (position_.x < (*it)->bound1.x && maxView.x < (*it)->bound1.x) { i++; continue; }
        if (position_.x > (*it)->bound2.x && maxView.x > (*it)->bound2.x) { i++; continue; }
		if (position_.y < (*it)->bound1.y && maxView.y < (*it)->bound1.y) { i++; continue; }
		if (position_.y > (*it)->bound2.y && maxView.y > (*it)->bound2.y) { i++; continue; }
		if (position_.z < (*it)->bound1.z && maxView.z < (*it)->bound1.z) { i++; continue; }
        if (position_.z > (*it)->bound2.z && maxView.z > (*it)->bound2.z) { i++; continue; }
        
        //if end of range of view is within box, there is intersection
        if (maxView.x > (*it)->bound1.x && maxView.x < (*it)->bound2.x &&
            maxView.y > (*it)->bound1.y && maxView.y < (*it)->bound2.y &&
            maxView.z > (*it)->bound1.z && maxView.z < (*it)->bound2.z) {
            SharedData::GetInstance()->canInteract = true;
            SharedData::GetInstance()->interactptr = *it;
			//SharedData::GetInstance()->interactnumber = i;
            break;
        }

        Vector3 intersect(0, 0, 0);
        if ( (GetIntersection(maxView.x - (*it)->bound1.x, position_.x - (*it)->bound1.x, maxView, position_, intersect) && InBox(intersect, (*it)->bound1, (*it)->bound2, 1)) ||
            (GetIntersection(maxView.y - (*it)->bound1.y, position_.y - (*it)->bound1.y, maxView, position_, intersect) && InBox(intersect, (*it)->bound1, (*it)->bound2, 2)) ||
            (GetIntersection(maxView.z - (*it)->bound1.z, position_.z - (*it)->bound1.z, maxView, position_, intersect) && InBox(intersect, (*it)->bound1, (*it)->bound2, 3)) ||
            (GetIntersection(maxView.x - (*it)->bound2.x, position_.x - (*it)->bound2.x, maxView, position_, intersect) && InBox(intersect, (*it)->bound1, (*it)->bound2, 1)) ||
            (GetIntersection(maxView.y - (*it)->bound2.y, position_.y - (*it)->bound2.y, maxView, position_, intersect) && InBox(intersect, (*it)->bound1, (*it)->bound2, 2)) ||
            (GetIntersection(maxView.z - (*it)->bound2.z, position_.z - (*it)->bound2.z, maxView, position_, intersect) && InBox(intersect, (*it)->bound1, (*it)->bound2, 3))) {
            SharedData::GetInstance()->canInteract = true;
            SharedData::GetInstance()->interactptr = *it;
			//SharedData::GetInstance()->interactnumber = i;
            break;
        }
		
    }

}
/******************************************************************************/
/*!
\brief  Gettng the hunger level of a player

\return hunger_
    Value that contains the hunger level of the player.
*/
/******************************************************************************/
int Player::getHunger()
{
    return hunger_;
}
/******************************************************************************/
/*!
\brief	Getting the Health of a player

\return health_
    Value which contains health of a player.
*/
/******************************************************************************/
int Player::getHealth()
{
    return health_;
}
/******************************************************************************/
/*!
\brief	Getting the Gold of a player

\return gold_
    Value which holds amount of gold player has.
*/
/******************************************************************************/
unsigned int Player::getGold()
{
    return gold_;
}
/******************************************************************************/
/*!
\brief	Setting the Gold of a player

\param moneh
    Value that holds the amount of gold player will get.
*/
/******************************************************************************/
void Player::setGold(int moneh)
{
	gold_ = moneh;
}
/******************************************************************************/
/*!
\brief	Checking Player Gold after receiving Change
    If player's gold and change added together is a negative value means player has no more money.
\param change
    Value which stores the change playe receive.
*/
/******************************************************************************/
void Player::changeGold(int change)
{
    if (gold_ + change <= 0) {
        setGold(0);
    }
    else {
        setGold(gold_ + change);
    }
}
/******************************************************************************/
/*!
\brief	Setting Hunger Level of Player
    Hunger will determine the speed of player.
\param hungee
    Value that stores the Hunger Level of Player
*/
/******************************************************************************/
void Player::setHunger(int hungee)
{
	hunger_ = hungee;
}
/******************************************************************************/
/*!
\brief	Check if Player is Dead
    Player is dead if health is below 0.
\return true
    Player health value is equal or below zero.
*/
/******************************************************************************/
bool Player::isDead()
{
    if (health_ <= 0) 
    {
        health_ = 0;
        return true;
    }
    else 
    {
        return false;
    }
}
/******************************************************************************/
/*!
\brief	Adding Item to Player Inventory.

\param itemID.
    Value that holds the Item ID of an item.
*/
/******************************************************************************/
int Player::addItem(int itemID)
{
	for (int i = 0; i < 8; i++)
	{
		if (inventory[i] == 0)
		{
			inventory[i] = itemID;
			return i;
		}
	}
	return -1;
}
/******************************************************************************/
/*!
\brief	Removing Item from Player Inventory

\param itemPos
    Values that holds the position the inventory marker is at.

\exception Selected space has no item
    It will not remove an empty space. (remove nothing)

\return
    Item ID of the item to be removed.
*/
/******************************************************************************/
int Player::removeItem(int itemPos)
{
	bool removed = false;
    int itemIDtoRemove = 0;
	for (int i = 0; i < 8; i++)
	{
		if ((i == itemPos) && (removed == false))
		{
            itemIDtoRemove = inventory[i];
			if (SharedData::GetInstance()->gamestate != GAME_STATE_SHOP)
			{
                useItem(itemIDtoRemove);
			}
			inventory[i] = 0;
			removed = true;
		}
		if (removed == true)
		{
			if (i == 7)
			{
				inventory[7] = 0;
				break;
			}
			inventory[i] = inventory[i + 1];
		}
	}
	if (removed == false)
	{
		return -1;
	}
	return itemIDtoRemove;
}
/******************************************************************************/
/*!
\brief	Player uses an Item
    Check what Item player is using and call out their functions
\param itemID
    value that stores item ID of an item to be used.
*/
/******************************************************************************/
void Player::useItem(int itemID)
{
	switch (itemID)
	{
	case 1: // hammer
		break;
	case 2: // temmeh
		break;
	case 3: // weed
		break;
	case 4: // spaghetti
		SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() - 18);
		break;
	case 5: // rotten spaghetti
		SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() - 9);
		break;
	case 6: // cookbook
		break;
	case 7: // anime poster
		break;
	case 8: // anime keychain
		break;
	case 9: // shovel
		break;
	case 10: // butterscotch pie
		SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() - 27);
		break;
	case 11: // board game
		break;
	case 12: // novel
		break;
	case 13: // jokebook
		break;
	}
}
/******************************************************************************/
/*!
\brief	Converting Inventory Item
    Changing the item in the player inventory to another item.
\param itemID1
    Value that stores the item ID that requires conversion.
\param itemID2
    Item ID Value to replace the item with.
*/
/******************************************************************************/
void Player::convertItem(int itemID1, int itemID2)
{
	for (int i = 0; i < 8; i++)
	{
		if (inventory[i] == itemID1)
		{
			inventory[i] = itemID2;
			return;
		}
	}
}
/******************************************************************************/
/*!
\brief	Checking what item player has

\param itemID
    value to check if player has that item in inventory
\return
    the number of same items that fulfill check requirements.
*/
/******************************************************************************/
int Player::itemHave(int itemID)
{
	int count = 0;
	for (int i = 0; i < 8; i++)
	{
		if (inventory[i] == itemID)
		{
			count++;
		}
	}
	return count;
}
/******************************************************************************/
/*!
\brief	Check Player Inventory Full
\return
    Returns a true if player's Inventory is full
*/
/******************************************************************************/
bool Player::invfull()
{
	if (inventory[7] != 0)
	{
		return true;
	}
	return false;
}
/******************************************************************************/
/*!
\brief	Player takes damage
*/
/******************************************************************************/
void Player::takeDamage()
{
    health_ -= 3;
}

/******************************************************************************/
/*!
\brief	NPC default constructor
\param name
name of non-playable character
\param pos
position of non-playable character
\param textDirectory
location where dialogue is stored
*/
/******************************************************************************/
NPC::NPC(std::string name, const Vector3& pos, std::string textDirectory) : Character(name, pos, 0), loveMeter_(0), numberCompliments_(0), minigameComplete_(false)
{
    //initialise map with the text files
    std::string dialogue;
    int convo;

    std::ifstream inData;
    std::string data;
    inData.open(textDirectory);

    if (inData.is_open()) {
        while (!inData.eof()) {
            std::getline(inData, data);

            std::stringstream dataStream(data);
            std::string line;

            //1st data
            std::getline(dataStream, line, ';');

            if (line == "#") {  //a comment
                continue;
            }
            convo = std::stoi(line);

            //2nd data
            std::getline(dataStream, line);
            dialogue = line;

            dialogues.insert(std::pair<CONVO_STATE, std::string>(static_cast<CONVO_STATE>(convo), dialogue));
        }
    }
    inData.close();
}

/******************************************************************************/
/*!
\brief	NPC default destructor
*/
/******************************************************************************/
NPC::~NPC()
{
}
/******************************************************************************/
/*!
\brief	Getting Love Meter of NPCs

\return loveMeter_
    The amount of Love for NPC.
*/
/******************************************************************************/
int NPC::getLoveMeter()
{
    return loveMeter_;
}
/******************************************************************************/
/*!
\brief	Setting Love Meter of NPCs

\param newLoveMeter
    A new value to re-set the love meter.
*/
/******************************************************************************/
void NPC::setLoveMeter(const int newLoveMeter)
{
    loveMeter_ = newLoveMeter;
    if (loveMeter_ > 100) {
        loveMeter_ = 100;
    }
    else if (loveMeter_ < 0) {
        loveMeter_ = 0;
    }
}
/******************************************************************************/
/*!
\brief	
Returns Speech of NPCs

\exception None

\return
Speech of NPC from a text file
*/
/******************************************************************************/
std::string NPC::Speech()
{
    //go through map to find the dialogue
    map<CONVO_STATE, std::string>::iterator it = dialogues.find(SharedData::GetInstance()->dialogueProcessor.convostate);
    return it->second;
}
/******************************************************************************/
/*!
\brief	Enemy default constructor
*/
/******************************************************************************/
Enemy::Enemy() : Character("rabbit", Vector3(5000, 25, 5000), 0), e_health_(450), e_damage_(5), yaw(180.f), iftakeDamage(false)
{

}
/******************************************************************************/
/*!
\brief	Enemy default constructor
\param name
name of the enemy
\param pos
position of enemy
*/
/******************************************************************************/
Enemy::Enemy(std::string name, const Vector3& pos) : Character("rabbit", Vector3(5000, 25, 5000), 0), e_health_(450), e_damage_(5), yaw(180.f)
{

}

/******************************************************************************/
/*!
\brief	Enemy default destructor
*/
/******************************************************************************/
Enemy::~Enemy()
{

}
/******************************************************************************/
/*!
\brief	Getting health of Enemy

\return
    Value which determines health of the enemy
*/
/******************************************************************************/
unsigned int Enemy::getHealth()
{
    return e_health_;
}
/******************************************************************************/
/*!
\brief	Setting health of Enemy

\param health
    Value that determines the health of the enemy
*/
/******************************************************************************/
void Enemy::setHealth(int health)
{
    e_health_ = health;
}
/******************************************************************************/
/*!
\brief	Enemy taking damage from source.

\param receiveddamage
    Value that determines damage dealt to Enemy.

\return None
*/
/******************************************************************************/
void Enemy::takeDamage(int receiveddamage)
{
    e_health_ -= receiveddamage;
}
/******************************************************************************/
/*!
\brief	Checking if Enemy is dead

\return TRUE if enemy health is below or equal 0.

*/
/******************************************************************************/
bool Enemy::isDead()
{
    if (e_health_ <= 0)
    {
        e_health_ = 0;
        return true;
    }
    return false;
}