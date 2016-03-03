/******************************************************************************/
/*!
\file	RabbitFight.cpp
\author 3000 Lines Studio
\brief
Functions for final Rabbit boss fight
*/
/******************************************************************************/

#include "RabbitFight.h"
//#include "SP2.h"
//#include "GL\glew.h"
//
//#include "shader.hpp"
//#include "Mtx44.h"
//#include "MeshBuilder.h"
//
//#include "Utility.h"
//#include "LoadTGA.h"
//
//#include <sstream>
//
//#include "Application.h"

/******************************************************************************/
/*!
\brief	RabbitFight default constructor
*/
/******************************************************************************/
RabbitFight::RabbitFight() : bulletDamage(10), bulletSpeed(25)
{

}

/******************************************************************************/
/*!
\brief	RabbitFight default destructor
*/
/******************************************************************************/
RabbitFight::~RabbitFight()
{

}

void RabbitFight::startUnleash()
{
    enemyPos.Set(0, 0, 0);
    playerPos.Set(10, 0, 10);

    int radius = 0;
    int diameter = 2 * radius;
    int pi = 3.14159;
    int circumference = 0;

    radius = 10;
    circumference = 2 * pi * radius;
}