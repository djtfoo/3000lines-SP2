#include "Vertex.h"

#ifndef LIGHT_H
#define LIGHT_H

struct Light
{
    enum LIGHT_TYPE
    {
        LIGHT_POINT = 0,
        LIGHT_DIRECTIONAL,
        LIGHT_SPOT,
    };

    LIGHT_TYPE type;
    //variables for spotlight
    Vector3 spotDirection;
    float cosCutoff;
    float cosInner;
    float exponent;

    Position position;
    Color color;
    float power;
    float kC, kL, kQ;
    
    Light()
    {
        type = LIGHT_POINT;
        position.Set(0, 0, 0);
        color.Set(1, 1, 1);

        power = 0;
        kC = 0;
        kL = 0;
        kQ = 0;
    }
};

#endif