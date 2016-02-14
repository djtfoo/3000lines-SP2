#include "Camera.h"
#include "MyMath.h"
#include "Application.h"
#include "Mtx44.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
    targetLock = true;
    move.Set(0, 0, 0);
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
    this->position = defaultPosition = pos;
    this->target = defaultTarget = target;
    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    this->up = defaultUp = right.Cross(view).Normalized();
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
    move.Set(0, 0, 0);
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
static float CAMSPEED = 40.f;

void Camera::Update(double dt, const Vector3& charPos)
{
    if (Application::IsKeyPressed('W')) {       //rotate up
        float pitch = (float)(-CAMSPEED * dt);
        Vector3 view = (target - position).Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation(pitch, right.x, right.y, right.z);
        position = rotation * position;
    }

    if (Application::IsKeyPressed('S')) {       //rotate down
        float pitch = (float)(CAMSPEED * dt);
        Vector3 view = (target - position).Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation(pitch, right.x, right.y, right.z);
        position = rotation * position;
    }

    if (Application::IsKeyPressed('A')) {       //rotate left
        float yaw = (float)(-CAMSPEED * dt);
        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation(yaw, 0, 1, 0);
        Mtx44 translate2;
        translate2.SetToIdentity();
        translate2.SetToTranslation(target.x, target.y, target.z);
        Mtx44 translate1;
        translate1.SetToIdentity();
        translate1.SetToTranslation(-target.x, -target.y, -target.z);
        Vector3 view = target - position;
        view = translate2 * rotation * translate1 * view;
        position = target - view;

        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
    }

    if (Application::IsKeyPressed('D')) {       //rotate right
        float yaw = (float)(CAMSPEED * dt);
        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation(yaw, 0, 1, 0);
        Mtx44 translate2;
        translate2.SetToIdentity();
        translate2.SetToTranslation(target.x, target.y, target.z);
        Mtx44 translate1;
        translate1.SetToIdentity();
        translate1.SetToTranslation(-target.x, -target.y, -target.z);
        Vector3 view = target - position;
        view = translate2 * rotation * translate1 * view;
        position = target - view;

        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
    }

    if (Application::IsKeyPressed('M')) {       //zoom out
        Vector3 view = (target - position).Normalized();
        position -= view * (float)(2 * CAMSPEED * dt);
    }

    if (Application::IsKeyPressed('N')) {       //zoom in
        Vector3 direction = target - position;
        if (direction.Length() > 5)
        {
            Vector3 view = (target - position).Normalized();
            position += view * (float)(2 * CAMSPEED * dt);
        }
    }

    if (Application::IsKeyPressed('E')) {   //move camera along positive direction of x-axis
        move.x += (float)(0.5f * CAMSPEED * dt);
    }
    if (Application::IsKeyPressed('Q')) {   //move camera along negative direction of x-axis
        move.x -= (float)(0.5f * CAMSPEED * dt);
    }
    if (Application::IsKeyPressed('Z')) {     //move camera along positive direction of y-axis
        move.y += (float)(0.5f * CAMSPEED * dt);
    }
    if (Application::IsKeyPressed('X')) {   //move camera along negative direction of y-axis
        move.y -= (float)(0.5f * CAMSPEED * dt);
    }
    if (Application::IsKeyPressed('G')) {   //move camera along positive direction of z-axis
        move.z += (float)(0.5f * CAMSPEED * dt);
    }
    if (Application::IsKeyPressed('F')) {   //move camera along negative direction of z-axis
        move.z -= (float)(0.5f * CAMSPEED * dt);
    }

    if (Application::IsKeyPressed('T')) {       //camera lock target on model
        targetLock = true;
        Vector3& dist = position - target;
        position = target + dist;
        target = charPos;
        move.Set(0, 0, 0);
    }
    if (Application::IsKeyPressed('Y')) {       //unlock target on model
        targetLock = false;
    }

    if (Application::IsKeyPressed('R')) {       //reset
        Reset();
    }
}

void Camera::TargetLock(const Vector3& charPos)
{
    Vector3& dist = position - target;
    target = charPos + move;
    position = target + dist;

    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    up = right.Cross(view).Normalized();
}