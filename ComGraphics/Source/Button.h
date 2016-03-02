#include "SharedData.h"

#ifndef BUTTON_H
#define BUTTON_H

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