/******************************************************************************/
/*!
\file	Vertex.h
\author 3000 Lines Studio
\brief
Vertex header file
*/
/******************************************************************************/

#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3.h"

/******************************************************************************/
/*!
Struct Position:
\brief	Stores 3D position
*/
/******************************************************************************/
struct Position
{
    float x, y, z;
    //Position(float x = 0, float y = 0, float z = 0) { Set(x, y, z); }     //not optimised code
    Position(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    void Set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

/******************************************************************************/
/*!
Struct Color:
\brief	A 4 by 4 matrix
*/
/******************************************************************************/
struct Color
{
    float r, g, b;
    Color(float r = 1, float g = 1, float b = 1)
    {
        Set(r, g, b);     //did not "clamp" the values between 0 and 1
    }
    void Set(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

/******************************************************************************/
/*!
Struct TexCoord:
\brief	Stores texture coordinates
*/
/******************************************************************************/
struct TexCoord
{
    float u, v;
    TexCoord(float u = 0, float v = 0)
    {
        Set(u, v);
    }
    void Set(float u, float v)
    {
        this->u = u;
        this->v = v;
    }
};

/******************************************************************************/
/*!
Struct Vertex:
\brief	Stores vertex information
*/
/******************************************************************************/
struct Vertex   //this sequence is important: Position followed by Color
{
    Position pos;
    Color color;
    Vector3 normal;
    TexCoord texCoord;
};

#endif