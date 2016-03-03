/******************************************************************************/
/*!
\file	Camera3.h
\author 3000 Lines Studio
\brief
Camera3 header file
*/
/******************************************************************************/

#include "Vector3.h"

#include <vector>

#ifndef CAMERA_3_H
#define CAMERA_3_H

/******************************************************************************/
/*!
Class Camera3:
\brief	Stores 3D camera information
*/
/******************************************************************************/
class Camera3
{
private:
    Vector3 defaultPosition;
    Vector3 defaultTarget;
    Vector3 defaultUp;

public:
    float active;       //To check if bullet active;
    float dy, dx;       //Speed of Bullet

    Vector3 position;
    Vector3 target;
    Vector3 up;

    Camera3();
    ~Camera3();
    void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    void Reset();       //reset to look at specific part at back of player
    void Update(double dt);
    void MoveCamera(double dt);
};

#endif