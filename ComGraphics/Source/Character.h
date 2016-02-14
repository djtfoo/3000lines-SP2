#include "Vector3.h"

#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
public:
    unsigned int health;
    Vector3 pos;
    bool takeDamage;
    void TakeDamage() {
        health -= 1;
    }
    bool isDead() {
        if (health == 0)
            return true;
        else
            return false;
    }
};

class Player : public Character
{
public:
    float pitch;
    float yaw;
    //float roll;
};

class Enemy : public Character
{
public:
    float translation;
    float yaw;
    //MOVE_DIR DIRECTION;   //for enemy to move around
};

struct Bullet
{
    Vector3 pos;
    Vector3 dir;
    float pitch, yaw;
    bool collided;
};

#endif