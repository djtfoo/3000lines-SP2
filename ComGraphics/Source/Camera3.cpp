#include "Camera3.h"
#include "MyMath.h"
#include "Application.h"
#include "Mtx44.h"

#include "SharedData.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera3::Camera3()
{
    phi = 15;
    theta = 20;
    distance = 30;
    active = 0;
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera3::~Camera3()
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
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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
void Camera3::Reset()
{
    position = defaultPosition;
    target = defaultTarget;
    up = defaultUp;
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/

static float ROTSPEED = 5.f;
static float CAMSPEED = 2.f * ROTSPEED;

void Camera3::Update(double dt)
{
    MoveCamera(dt);

    if (Application::IsKeyPressed('R')) {       //reset
        Reset();
    }
}

void Camera3::MoveCamera(double dt)
{
    if (SharedData::GetInstance()->cursor_newxpos != SharedData::GetInstance()->cursor_xpos) {
        double diff_xpos = SharedData::GetInstance()->cursor_xpos - SharedData::GetInstance()->cursor_newxpos;
        SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;

        theta -= diff_xpos * ROTSPEED * dt;
    }

    if (SharedData::GetInstance()->cursor_newypos != SharedData::GetInstance()->cursor_ypos) {
        double diff_ypos = SharedData::GetInstance()->cursor_ypos - SharedData::GetInstance()->cursor_newypos;
        SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;

        phi -= diff_ypos * ROTSPEED * dt;
        if (phi > 50) {
            phi = 50;
        }
        else if (phi < -30) {
            phi = -30;
        }
    }

    target = SharedData::GetInstance()->player->position_;
    target.y += 25.0f;

    position.x = distance * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)) + target.x;
    position.y = distance * sin(Math::DegreeToRadian(phi)) + target.y;
    position.z = distance * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)) + target.z;

    Vector3 view = (target - position).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    up = right.Cross(view).Normalized();
}
