/******************************************************************************/
/*!
\file	MiniMapCamera.h
\author 3000 Lines Studio
\brief
Minimap header file
*/
/******************************************************************************/

#include "Vector3.h"

#include "Character.h"
#include "Collision.h"

#include <vector>

#ifndef MINIMAPCAMERA_H
#define MINIMAPCAMERA_H

/******************************************************************************/
/*!
Class Minimapcamera:
\brief	stores information for minimap
*/
/******************************************************************************/
class Minimapcamera
{
private:
    Vector3 defaultPos;
    Vector3 defaultTar;
    Vector3 defaultUP;
    
public:
    float phi;      //angle of elevation
    float theta;
    float dist;

    Vector3 pos;
    Vector3 targ;
    Vector3 UP;

    Minimapcamera();
    ~Minimapcamera();

    void Init(const Vector3& position, const Vector3& target, const Vector3& up);
    void Update(double dt);
    void MoveCam(double dt);
};

#endif /* MINIMAPCAMERA_H */