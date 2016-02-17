#include "Character.h"

#include "Application.h"
#include "SharedData.h"

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

Player::Player(std::string name) : Character(name, Vector3(0, 0, 0), 0), hunger_(0), health_(100), gold_(0)
{
}

Player::~Player()
{
}

void Player::Walk(double dt)
{
    if (Application::IsKeyPressed('W')) {   //player moves forward
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        position_.x -= (float)(40 * sin(Math::DegreeToRadian(direction_)) * dt);
        position_.z -= (float)(40 * cos(Math::DegreeToRadian(direction_)) * dt);
    }
    if (Application::IsKeyPressed('F')) {
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        position_.x -= (float)(130 * sin(Math::DegreeToRadian(direction_)) * dt);
        position_.z -= (float)(130 * cos(Math::DegreeToRadian(direction_)) * dt);
    }
    if (Application::IsKeyPressed('S')) {   //player moves backward
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        position_.x += (float)(35 * sin(Math::DegreeToRadian(direction_)) * dt);
        position_.z += (float)(35 * cos(Math::DegreeToRadian(direction_)) * dt);
    }
    if (Application::IsKeyPressed('A')) {   //strafe left
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        Vector3 right = view.Cross(Vector3(0, 1, 0));
        right.y = 0;
        right.Normalize();
        position_.x -= (float)(50 * right.x * dt);
        position_.z -= (float)(50 * right.z * dt);
    }
    if (Application::IsKeyPressed('D')) {   //strafe right
        direction_ = 90 - SharedData::GetInstance()->camera->theta;
        Vector3 view = (SharedData::GetInstance()->camera->target - SharedData::GetInstance()->camera->position).Normalized();
        Vector3 right = view.Cross(Vector3(0, 1, 0));
        right.y = 0;
        right.Normalize();
        position_.x += (float)(50 * right.x * dt);
        position_.z += (float)(50 * right.z * dt);
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