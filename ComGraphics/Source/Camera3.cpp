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
    distance = 25;
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
static float CAMSPEED = 800.f;
static float FLYSPEED = 800.f;
static float ROTSPEED = 5.f;

void Camera3::Update(double dt)
{
    MoveCamera(dt);

    /*if (Application::IsKeyPressed('W')) {   //move forward
        Vector3 view = (target - position);
        view.y = 0;
        view = view.Normalized();

        float newX = target.x + view.x * dt * CAMSPEED;
        float newZ = target.z + view.z * dt * CAMSPEED;

        bool xMovement = true, zMovement = true;
        //collision with walls
        for (unsigned i = 0; i < collisionWall.size(); ++i) {
            if (collisionWall[i].dir == WALL_LEFT) {
                if (newX < collisionWall[i].coordinate) {
                    xMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_RIGHT) {
                if (newX > collisionWall[i].coordinate) {
                    xMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_FRONT) {
                if (newZ < collisionWall[i].coordinate) {
                    zMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_BACK) {
                if (newZ > collisionWall[i].coordinate) {
                    zMovement = false;
                }
            }
        }

        //collision with objects
        for (unsigned i = 0; i < collisionItem.size(); ++i) {
            if (target.x > collisionItem[i].minX && target.x < collisionItem[i].maxX) {
                if (newZ > collisionItem[i].minZ && newZ < collisionItem[i].maxZ) {
                    zMovement = false;
                }
            }
            if (target.z > collisionItem[i].minZ && target.z < collisionItem[i].maxZ) {
                if (newX > collisionItem[i].minX && newX < collisionItem[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            target.x += (float)(view.x * dt * CAMSPEED);
            position.x += (float)(view.x * dt * CAMSPEED);
        }
        if (zMovement) {
            target.z += (float)(view.z * dt * CAMSPEED);
            position.z += (float)(view.z * dt * CAMSPEED);
        }
    }

    if (Application::IsKeyPressed('S')) {   //move backward
        Vector3 view = (target - position);
        view.y = 0;
        view = view.Normalized();

        float newX = target.x - view.x * dt * CAMSPEED;
        float newZ = target.z - view.z * dt * CAMSPEED;

        bool xMovement = true, zMovement = true;
        //collision with walls
        for (unsigned i = 0; i < collisionWall.size(); ++i) {
            if (collisionWall[i].dir == WALL_LEFT) {
                if (newX < collisionWall[i].coordinate) {
                    xMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_RIGHT) {
                if (newX > collisionWall[i].coordinate) {
                    xMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_FRONT) {
                if (newZ < collisionWall[i].coordinate) {
                    zMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_BACK) {
                if (newZ > collisionWall[i].coordinate) {
                    zMovement = false;
                }
            }
        }

        //collision with objects
        for (unsigned i = 0; i < collisionItem.size(); ++i) {
            if (target.x > collisionItem[i].minX && target.x < collisionItem[i].maxX) {
                if (newZ > collisionItem[i].minZ && newZ < collisionItem[i].maxZ) {
                    zMovement = false;
                }
            }
            if (target.z > collisionItem[i].minZ && target.z < collisionItem[i].maxZ) {
                if (newX > collisionItem[i].minX && newX < collisionItem[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            target.x -= (float)(view.x * dt * CAMSPEED);
            position.x -= (float)(view.x * dt * CAMSPEED);
        }
        if (zMovement) {
            target.z -= (float)(view.z * dt * CAMSPEED);
            position.z -= (float)(view.z * dt * CAMSPEED);
        }
    }

    if (Application::IsKeyPressed('A')) {   //strafe left
        Vector3 view = (target - position).Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();

        float newX = target.x - right.x * dt * CAMSPEED;
        float newZ = target.z - right.z * dt * CAMSPEED;

        bool xMovement = true, zMovement = true;
        //collision with walls
        for (unsigned i = 0; i < collisionWall.size(); ++i) {
            if (collisionWall[i].dir == WALL_LEFT) {
                if (newX < collisionWall[i].coordinate) {
                    xMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_RIGHT) {
                if (newX > collisionWall[i].coordinate) {
                    xMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_FRONT) {
                if (newZ < collisionWall[i].coordinate) {
                    zMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_BACK) {
                if (newZ > collisionWall[i].coordinate) {
                    zMovement = false;
                }
            }
        }

        //collision with objects
        for (unsigned i = 0; i < collisionItem.size(); ++i) {
            if (target.x > collisionItem[i].minX && target.x < collisionItem[i].maxX) {
                if (newZ > collisionItem[i].minZ && newZ < collisionItem[i].maxZ) {
                    zMovement = false;
                }
            }
            if (target.z > collisionItem[i].minZ && target.z < collisionItem[i].maxZ) {
                if (newX > collisionItem[i].minX && newX < collisionItem[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            target.x -= (float)(right.x * dt * CAMSPEED);
            position.x -= (float)(right.x * dt * CAMSPEED);
        }
        if (zMovement) {
            target.z -= (float)(right.z * dt * CAMSPEED);
            position.z -= (float)(right.z * dt * CAMSPEED);
        }
    }

    if (Application::IsKeyPressed('D')) {   //strafe right
        Vector3 view = (target - position).Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();

        float newX = target.x + right.x * dt * CAMSPEED;
        float newZ = target.z + right.z * dt * CAMSPEED;

        bool xMovement = true, zMovement = true;
        //collision with walls
        for (unsigned i = 0; i < collisionWall.size(); ++i) {
            if (collisionWall[i].dir == WALL_LEFT) {
                if (newX < collisionWall[i].coordinate) {
                    xMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_RIGHT) {
                if (newX > collisionWall[i].coordinate) {
                    xMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_FRONT) {
                if (newZ < collisionWall[i].coordinate) {
                    zMovement = false;
                }
            }
            else if (collisionWall[i].dir == WALL_BACK) {
                if (newZ > collisionWall[i].coordinate) {
                    zMovement = false;
                }
            }
        }

        //collision with objects
        for (unsigned i = 0; i < collisionItem.size(); ++i) {
            if (target.x > collisionItem[i].minX && target.x < collisionItem[i].maxX) {
                if (newZ > collisionItem[i].minZ && newZ < collisionItem[i].maxZ) {
                    zMovement = false;
                }
            }
            if (target.z > collisionItem[i].minZ && target.z < collisionItem[i].maxZ) {
                if (newX > collisionItem[i].minX && newX < collisionItem[i].maxX) {
                    xMovement = false;
                }
            }
        }

        if (xMovement) {
            target.x += (float)(right.x * dt * CAMSPEED);
            position.x += (float)(right.x * dt * CAMSPEED);
        }
        if (zMovement) {
            target.z += (float)(right.z * dt * CAMSPEED);
            position.z += (float)(right.z * dt * CAMSPEED);
        }
    }

    //move up/down stairs
    if (stairs.dir == STAIR_X) {
        if (position.x < stairs.minVal) {
            Vector3 view = (target - position).Normalized();
            position.y = stairs.minY;
            target = position + view;
        }
        else if (position.x > stairs.minVal && position.x < stairs.maxVal) {
            for (int i = 0; i <= stairs.steps; ++i) {
                if (position.x > stairs.minVal + i * stairs.incrementVal) {
                    Vector3 view = (target - position).Normalized();
                    position.y = stairs.minY + (i + 1) * stairs.incrementY;
                    target = position + view;
                }
            }
        }
    }
    else if (stairs.dir == STAIR_Z) {
        //left empty (for now) as not needed for assignment 3
    }*/

    if (Application::IsKeyPressed('R')) {       //reset
        Reset();
    }
}

void Camera3::MoveCamera(double dt)
{
    if (Application::cursor_newxpos != Application::cursor_xpos) {
        double diff_xpos = Application::cursor_xpos - Application::cursor_newxpos;
        Application::cursor_xpos = Application::cursor_newxpos;

        theta -= diff_xpos * ROTSPEED * dt;
    }

    if (Application::cursor_newypos != Application::cursor_ypos) {
        double diff_ypos = Application::cursor_ypos - Application::cursor_newypos;
        Application::cursor_ypos = Application::cursor_newypos;

        phi -= diff_ypos * ROTSPEED * dt;
        if (phi > 60) {
            phi = 60;
        }
        else if (phi < -30) {
            phi = -30;
        }
    }

    if (Application::IsKeyPressed('Q')) {
        distance += ROTSPEED * dt;
    }

    if (Application::IsKeyPressed('E')) {
        if (distance > 1 + ROTSPEED * dt) {
            distance -= ROTSPEED * dt;
        }
    }

    target = SharedData::GetInstance()->player->position_;
    target.y += 20.0f;

    position.x = distance * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)) + target.x;
    position.y = distance * sin(Math::DegreeToRadian(phi)) + target.y;
    position.z = distance * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)) + target.z;
}
