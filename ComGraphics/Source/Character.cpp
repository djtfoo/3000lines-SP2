#include "Character.h"

#include "Application.h"
#include "SharedData.h"

#include <sstream>
#include <fstream>

Character::Character(std::string name, Vector3 position, float direction) : name_(name), position_(position), direction_(0)
{
}

Character::~Character()
{
}

std::string Character::getName()
{
    return name_;
}

Player::Player(std::string name) : Character(name, Vector3(0, 0, 0), 0), hunger_(0), health_(100), gold_(0), hat(nullptr), top(nullptr), bottoms(nullptr), invselect(0)
{
	for (int i = 0; i < 8; i++)
	{
		inventory[i] = 0;
	}
}

Player::~Player()
{
}

void Player::Walk(double dt)
{
    bool xMovement = true, zMovement = true;
    float newX = position_.x;
    float newZ = position_.z;

    if (Application::IsKeyPressed('W')) {   //player moves forward
        direction_ = 90 - SharedData::GetInstance()->camera->theta;

        newX -= 40 * sin(Math::DegreeToRadian(direction_)) * dt;
        newZ -= 40 * cos(Math::DegreeToRadian(direction_)) * dt;
    }

    if (Application::IsKeyPressed('F')) {   //sped up moving forward
        direction_ = 90 - SharedData::GetInstance()->camera->theta;

        newX -= 130 * sin(Math::DegreeToRadian(direction_)) * dt;
        newZ -= 130 * cos(Math::DegreeToRadian(direction_)) * dt;
    }

    if (Application::IsKeyPressed('S')) {   //player moves backward
        direction_ = 90 - SharedData::GetInstance()->camera->theta;

        newX += 40 * sin(Math::DegreeToRadian(direction_)) * dt;
        newZ += 40 * cos(Math::DegreeToRadian(direction_)) * dt;
    }

    if (Application::IsKeyPressed('A')) {   //strafe left
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        Vector3 right = view.Cross(Vector3(0, 1, 0));
        right.y = 0;
        right.Normalize();

        newX -= 50 * right.x * dt;
        newZ -= 50 * right.z * dt;
    }

    if (Application::IsKeyPressed('D')) {   //strafe right
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        Vector3 right = view.Cross(Vector3(0, 1, 0));
        right.y = 0;
        right.Normalize();

        newX += 50 * right.x * dt;
        newZ += 50 * right.z * dt;
    }

    CheckCollision(newX, newZ, xMovement, zMovement);

    if (xMovement) {
        position_.x = newX;
    }
    if (zMovement) {
        position_.z = newZ;
    }

}

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

bool inline GetIntersection(float dist1, float dist2, Vector3 maxView, Vector3 playerPos, Vector3& intersect)
{
    if ((dist1 * dist2) >= 0.0f)
        return false;
    if (dist1 == dist2)
        return false;
    
    intersect = maxView + (playerPos - maxView) * (-dist1 / (dist2 - dist1));
    return true;
}

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

void Player::CheckInteraction()
{
    SharedData::GetInstance()->canInteract = false;

    vector<Interaction*> temp = SharedData::GetInstance()->interactionItems;
    Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
    Vector3 maxView = position_ + 75 * view;
	int i = 0;
    for (vector<Interaction*>::iterator it = temp.begin(); it != temp.end(); ++it) {
        
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
			SharedData::GetInstance()->interactnumber = i;
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
			SharedData::GetInstance()->interactnumber = i;
            break;
        }
		
    }

}

int Player::getHunger()
{
    return hunger_;
}

unsigned int Player::getHealth()
{
    return health_;
}

unsigned int Player::getGold()
{
    return gold_;
}

void Player::setHunger(int hungee)
{
	hunger_ = hungee;
}

bool Player::isDead()
{
    if (getHealth() == 0) {
        return true;
    }
    else {
        return false;
    }
}

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

int Player::removeItem(int itemPos)
{
	bool removed = false;
	for (int i = 0; i < 8; i++)
	{
		if ((i == itemPos) && (removed == false))
		{
			useItem(inventory[i]);
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
	return 1;
}

void Player::useItem(int itemID)
{
	switch (itemID)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		SharedData::GetInstance()->player->setHunger(SharedData::GetInstance()->player->getHunger() - 9);
		break;
	}
}

bool Player::itemHave(int itemID)
{
	for (int i = 0; i < 8; i++)
	{
		if (inventory[i] == itemID)
		{
			return true;
		}
	}
	return false;
}

bool Player::invfull()
{
	if (inventory[7] != 0)
	{
		return false;
	}
	return true;
}

NPC::NPC(std::string name, const Vector3& pos, std::string textDirectory) : Character(name, pos, 0)
{
    //initialise map with the text file's directory
    
    //TESTING EXAMPLE
    dialogues.insert(std::pair<CONVO_STATE, std::string>(CONVO_INTRO, "Eyyy sup"));
    dialogues.insert(std::pair<CONVO_STATE, std::string>(CONVO_NEUTRAL, "Anything you wanna do?"));
    dialogues.insert(std::pair<CONVO_STATE, std::string>(CONVO_GIFT, "Aww ty"));
    dialogues.insert(std::pair<CONVO_STATE, std::string>(CONVO_COMPLIMENT, "I shy"));
    dialogues.insert(std::pair<CONVO_STATE, std::string>(CONVO_STARTMINIGAME, "Play mini-game"));
}

NPC::~NPC()
{
}

std::string NPC::Speech()
{
    //go through map to find the dialogue
    map<CONVO_STATE, std::string>::iterator it = dialogues.find(SharedData::GetInstance()->dialogueProcessor.convostate);
    return it->second;
}