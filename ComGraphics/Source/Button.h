/******************************************************************************/
/*!
\file	Button.h
\author 3000 Lines Studio
\brief
Button header file
*/
/******************************************************************************/

#include "SharedData.h"

#ifndef BUTTON_H
#define BUTTON_H

/******************************************************************************/
/*!
Struct Button:
\brief	Stores button information
*/
/******************************************************************************/
struct Button
{
    float minX, maxX, minY, maxY;

    void Set(float factorMinX, float factorMaxX, float factorMinY, float factorMaxY)
    {
        minX = SharedData::GetInstance()->width * factorMinX;
        maxX = SharedData::GetInstance()->width * factorMaxX;

        minY = SharedData::GetInstance()->height * factorMinY;
        maxY = SharedData::GetInstance()->height * factorMaxY;
    }
};

#endif