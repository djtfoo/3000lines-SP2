/******************************************************************************/
/*!
\file	RabbitFight.h
\author 3000 Lines Studio
\brief
RabbitFight header file
*/
/******************************************************************************/

#ifndef RABBITFIGHT_H
#define RABBITFIGHT_H

#include "Vector3.h"

/******************************************************************************/
/*!
Class RabbitFight:
\brief	Stores information for the Rabbit boss fight
*/
/******************************************************************************/
class RabbitFight
{
public:
    RabbitFight();
    virtual ~RabbitFight();

    void startUnleash(); //Rabbit first appear
    void bulletSpray(); //Spray bullets at player
    void bulletBurst(); //Bullet bursts out upon landing
    void bulletShield(); //Creates a Circle of Bullets, then fly to player.

    Vector3 enemyPos;
    Vector3 playerPos;

private:
    int bulletDamage;
    int bulletSpeed;
    RabbitFight* bulletTypes;
};

#endif /* RABBITFIGHT_H */