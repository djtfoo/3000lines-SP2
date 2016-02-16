#include "Character.h"

#include "Application.h"

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
        position_.x -= (float)(40 * sin(Math::DegreeToRadian(direction_)) * dt);
        position_.z -= (float)(40 * cos(Math::DegreeToRadian(direction_)) * dt);
    }
    if (Application::IsKeyPressed('S')) {   //player moves backward
        position_.x += (float)(35 * sin(Math::DegreeToRadian(direction_)) * dt);
        position_.z += (float)(35 * cos(Math::DegreeToRadian(direction_)) * dt);
    }
    if (Application::IsKeyPressed('A')) {   //turn left
        direction_ += (float)(230 * dt);
    }
    if (Application::IsKeyPressed('D')) {   //turn right
        direction_ -= (float)(230 * dt);
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