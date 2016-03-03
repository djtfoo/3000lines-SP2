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
    Vector3 defaultPosition;        ///< Default Position of Camera
    Vector3 defaultTarget;          ///< Default Target of Camera
    Vector3 defaultUp;              ///< Default Up Vector of Camera

public:
    float active;                   ///< To check if bullet active;
    float dy, dx;                   ///< Speed of Bullet

    Vector3 position;               ///< Position of the camera
    Vector3 target;                 ///< Target of the camera
    Vector3 up;                     ///< Up Vector of Camera

    Camera3();                      ///< Default Constructor
    ~Camera3();                     ///< Default Destructor
    void Init(const Vector3& pos, const Vector3& target, const Vector3& up);    ///< Initializer of Camera
    void Reset();                   ///< Reset to look at specific part at back of player
    void Update(double dt);         ///< Updating Camera3
    void MoveCamera(double dt);     ///< Moving the Camera3
};

#endif