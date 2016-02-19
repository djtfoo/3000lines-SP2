#include "Character.h"

#include "Application.h"
#include "SharedData.h"

#include <iostream>
#include <sstream>

Character::Character(std::string name, Vector3 position, float direction) : name_(name), position_(Vector3(0, 0, 0)), direction_(0)
{
}

Character::~Character()
{
}

std::string Character::getName()
{
    return name_;
}

Player::Player(std::string name) : Character(name, Vector3(0, 0, 0), 0), hunger_(0), health_(100), gold_(0), invselect(0)
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
    if (Application::IsKeyPressed('W')) {   //player moves forward
        direction_ = 90 - SharedData::GetInstance()->camera->theta;

        bool xMovement = true, zMovement = true;
        float newX = position_.x - 40 * sin(Math::DegreeToRadian(direction_)) * dt;
        float newZ = position_.z - 40 * cos(Math::DegreeToRadian(direction_)) * dt;
        for (size_t i = 0; i < SharedData::GetInstance()->collisionItems.size(); ++i) {
            if (position_.x > SharedData::GetInstance()->collisionItems[i].minX && position_.x < SharedData::GetInstance()->collisionItems[i].maxX) {
                if (newZ > SharedData::GetInstance()->collisionItems[i].minZ && newZ < SharedData::GetInstance()->collisionItems[i].maxZ) {
                    zMovement = false;
                }
            }
            else if (position_.z > SharedData::GetInstance()->collisionItems[i].minZ && position_.z < SharedData::GetInstance()->collisionItems[i].maxZ) {
                if (newX > SharedData::GetInstance()->collisionItems[i].minX && newX < SharedData::GetInstance()->collisionItems[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            position_.x -= (float)(40 * sin(Math::DegreeToRadian(direction_)) * dt);
        }
        if (zMovement) {
            position_.z -= (float)(40 * cos(Math::DegreeToRadian(direction_)) * dt);
        }
    }
    if (Application::IsKeyPressed('F')) {
        direction_ = 90 - SharedData::GetInstance()->camera->theta;

        bool xMovement = true, zMovement = true;
        float newX = position_.x - 130 * sin(Math::DegreeToRadian(direction_)) * dt;
        float newZ = position_.z - 130 * cos(Math::DegreeToRadian(direction_)) * dt;
        for (size_t i = 0; i < SharedData::GetInstance()->collisionItems.size(); ++i) {
            if (position_.x > SharedData::GetInstance()->collisionItems[i].minX && position_.x < SharedData::GetInstance()->collisionItems[i].maxX) {
                if (newZ > SharedData::GetInstance()->collisionItems[i].minZ && newZ < SharedData::GetInstance()->collisionItems[i].maxZ) {
                    zMovement = false;
                }
            }
            else if (position_.z > SharedData::GetInstance()->collisionItems[i].minZ && position_.z < SharedData::GetInstance()->collisionItems[i].maxZ) {
                if (newX > SharedData::GetInstance()->collisionItems[i].minX && newX < SharedData::GetInstance()->collisionItems[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            position_.x -= (float)(130 * sin(Math::DegreeToRadian(direction_)) * dt);
        }
        if (zMovement) {
            position_.z -= (float)(130 * cos(Math::DegreeToRadian(direction_)) * dt);
        }
    }
    if (Application::IsKeyPressed('S')) {   //player moves backward
        direction_ = 90 - SharedData::GetInstance()->camera->theta;

        bool xMovement = true, zMovement = true;
        float newX = position_.x + 40 * sin(Math::DegreeToRadian(direction_)) * dt;
        float newZ = position_.z + 40 * cos(Math::DegreeToRadian(direction_)) * dt;
        for (size_t i = 0; i < SharedData::GetInstance()->collisionItems.size(); ++i) {
            if (position_.x > SharedData::GetInstance()->collisionItems[i].minX && position_.x < SharedData::GetInstance()->collisionItems[i].maxX) {
                if (newZ > SharedData::GetInstance()->collisionItems[i].minZ && newZ < SharedData::GetInstance()->collisionItems[i].maxZ) {
                    zMovement = false;
                }
            }
            else if (position_.z > SharedData::GetInstance()->collisionItems[i].minZ && position_.z < SharedData::GetInstance()->collisionItems[i].maxZ) {
                if (newX > SharedData::GetInstance()->collisionItems[i].minX && newX < SharedData::GetInstance()->collisionItems[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            position_.x += (float)(40 * sin(Math::DegreeToRadian(direction_)) * dt);
        }
        if (zMovement) {
            position_.z += (float)(40 * cos(Math::DegreeToRadian(direction_)) * dt);
        }
    }
    if (Application::IsKeyPressed('A')) {   //strafe left
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        Vector3 right = view.Cross(Vector3(0, 1, 0));
        right.y = 0;
        right.Normalize();

        bool xMovement = true, zMovement = true;
        float newX = position_.x - 50 * right.x * dt;
        float newZ = position_.z - 50 * right.z * dt;
        for (size_t i = 0; i < SharedData::GetInstance()->collisionItems.size(); ++i) {
            if (position_.x > SharedData::GetInstance()->collisionItems[i].minX && position_.x < SharedData::GetInstance()->collisionItems[i].maxX) {
                if (newZ > SharedData::GetInstance()->collisionItems[i].minZ && newZ < SharedData::GetInstance()->collisionItems[i].maxZ) {
                    zMovement = false;
                }
            }
            else if (position_.z > SharedData::GetInstance()->collisionItems[i].minZ && position_.z < SharedData::GetInstance()->collisionItems[i].maxZ) {
                if (newX > SharedData::GetInstance()->collisionItems[i].minX && newX < SharedData::GetInstance()->collisionItems[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            position_.x -= (float)(50 * right.x * dt);
        }
        if (zMovement) {
            position_.z -= (float)(50 * right.z * dt);
        }
    }
    if (Application::IsKeyPressed('D')) {   //strafe right
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        Vector3 right = view.Cross(Vector3(0, 1, 0));
        right.y = 0;
        right.Normalize();

        bool xMovement = true, zMovement = true;
        float newX = position_.x + 50 * right.x * dt;
        float newZ = position_.z + 50 * right.z * dt;

        vector<ItemCollision> temp = SharedData::GetInstance()->collisionItems;

        for (size_t i = 0; i < temp.size(); ++i) {
            if (position_.x > temp[i].minX && position_.x < temp[i].maxX) {
                if (newZ > temp[i].minZ && newZ < temp[i].maxZ) {
                    zMovement = false;
                }
            }
            else if (position_.z > temp[i].minZ && position_.z < temp[i].maxZ) {
                if (newX > temp[i].minX && newX < temp[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            position_.x += (float)(50 * right.x * dt);
        }
        if (zMovement) {
            position_.z += (float)(50 * right.z * dt);
        }
    }
}

void Player::CheckInteraction()
{
    SharedData::GetInstance()->canInteract = false;

    vector<Interaction*> temp = SharedData::GetInstance()->interactionItems;
    Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
    for (size_t i = 0; i < temp.size(); ++i) {
        Vector3 distance = temp[i]->middlePoint - position_;
        if (distance.LengthSquared() > 5625) {  //player too far away
            continue;
        }
        float dotProduct = distance.Dot(view);
        //Vector3 projected = dotProduct / distance.LengthSquared() * distance;  //project view vector onto distance vector
        float distLength = distance.Length();
        float angle = Math::RadianToDegree(acos(dotProduct / distLength));
        float magnitude = distLength / cos(Math::DegreeToRadian(angle));
        Vector3 projected = magnitude * view + position_;

        if (projected.x >(temp[i]->middlePoint.x - temp[i]->distX) && projected.x < (temp[i]->middlePoint.x + temp[i]->distX) && projected.y >(temp[i]->middlePoint.y - temp[i]->distY) && projected.y < (temp[i]->middlePoint.y + temp[i]->distY) && projected.z >(temp[i]->middlePoint.z - temp[i]->distZ) && projected.z < (temp[i]->middlePoint.z + temp[i]->distZ)) {
            std::cout << projected.x << " " << projected.y << " " << projected.z << std::endl;
            SharedData::GetInstance()->canInteract = true;
            SharedData::GetInstance()->interactptr = temp[i];
            break;
        }
    }

}

unsigned int Player::getHunger()
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

bool Player::isDead()
{
    if (getHealth() == 0) {
        return true;
    }
    else {
        return false;
    }
}

std::string Player::addItem(int itemID)
{
	for (int i = 0; i < 8; i++)
	{
		if (inventory[i] == 0)
		{
			inventory[i] = itemID;
			std::stringstream feedbackreport;
			feedbackreport << "The item " << itemID << " has been added to inventory.";
			std::string returner = feedbackreport.str();
			return returner;
		}
	}
	return "Inventory is full";
}

std::string Player::removeItem(int itemPos)
{
	bool removed = false;
	std::stringstream feedbackreport;
	for (int i = 0; i < 8; i++)
	{
		if ((i == itemPos) && (removed == false))
		{
			feedbackreport << "The item at " << itemPos << " has been removed from inventory.";
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
		feedbackreport << "Error.";
	}
	std::string returner = feedbackreport.str();
	return returner;
}