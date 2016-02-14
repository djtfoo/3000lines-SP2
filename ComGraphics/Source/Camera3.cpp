#include "Camera3.h"
#include "MyMath.h"
#include "Application.h"
#include "Mtx44.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera3::Camera3()
{
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
static float ROTSPEED = 3.f;

void Camera3::Update(double dt)
{
    /*if (Application::IsKeyPressed('M')) {       //zoom out
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
    }*/

    MoveCamera(dt);

    if (Application::IsKeyPressed('W')) {   //move forward
        Vector3 view = (target - position);
        view.y = 0;
        view = view.Normalized();

        target.x += view.x;
        position.x += view.x;
        target.z += view.z;
        position.z += view.z;
    }

    if (Application::IsKeyPressed('S')) {   //move backward
        Vector3 view = (target - position);
        view.y = 0;
        view = view.Normalized();
        target -= view;
        position -= view;
    }

    if (Application::IsKeyPressed('A')) {   //strafe left
        Vector3 view = (target - position).Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        position -= right;
        target -= right;
    }

    if (Application::IsKeyPressed('D')) {   //strafe right
        Vector3 view = (target - position);
        view.y = 0;
        view = view.Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        position += right;
        target += right;
    }

    if (Application::IsKeyPressed('Z')) {     //move camera along positive direction of y-axis
        position.y += (float)(0.5f * CAMSPEED * dt);
        target.y += (float)(0.5f * CAMSPEED * dt);
    }
    if (Application::IsKeyPressed('X')) {   //move camera along negative direction of y-axis
        position.y -= (float)(0.5f * CAMSPEED * dt);
        target.y -= (float)(0.5f * CAMSPEED * dt);
    }

    if (Application::IsKeyPressed('R')) {       //reset
        Reset();
    }
}

void Camera3::Update(double dt, const std::vector<WallCollision>& collisionWall, const std::vector<ItemCollision>& collisionItem)
{
    MoveCamera(dt);

    if (Application::IsKeyPressed('W')) {   //move forward
        Vector3 view = (target - position);
        view.y = 0;
        view = view.Normalized();

        float newX = target.x + view.x * dt * CAMSPEED;
        float newZ = target.z + view.z * dt * CAMSPEED;

        bool xMovement = true, zMovement = true;
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

}

void Camera3::Update(double dt, const std::vector<WallCollision>& collisionWall, const std::vector<ItemCollision>& collisionItem, const StairsCollision& stairs)
{
    MoveCamera(dt);

    if (Application::IsKeyPressed('W')) {   //move forward
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
    }
}

void Camera3::MoveCamera(double dt)
{
    if (Application::cursor_newxpos != Application::cursor_xpos) {
        double diff_xpos = Application::cursor_xpos - Application::cursor_newxpos;
        Application::cursor_xpos = Application::cursor_newxpos;

        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation((float)(diff_xpos * ROTSPEED * dt), 0, 1, 0);
        Vector3 view = target - position;
        view = rotation * view;
        target = position + view;

        view = view.Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
    }

    if (Application::cursor_newypos != Application::cursor_ypos) {
        double diff_ypos = Application::cursor_ypos - Application::cursor_newypos;
        Application::cursor_ypos = Application::cursor_newypos;

        Vector3 view = (target - position).Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation((float)(diff_ypos * ROTSPEED * dt), right.x, right.y, right.z);
        view = rotation * view;
        if (view.y < 0.8f && view.y > -0.9f) {
            target = position + view;
        }
    }
}

void Camera3::LimitedMoveCamera(double dt)
{
    if (Application::cursor_newxpos != Application::cursor_xpos) {
        double diff_xpos = Application::cursor_xpos - Application::cursor_newxpos;
        Application::cursor_xpos = Application::cursor_newxpos;

        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation((float)(diff_xpos * ROTSPEED * dt), 0, 1, 0);
        Vector3 view = target - position;
        view = rotation * view;
        if (view.x < 0.4f && view.x > -0.4f) {
            target = position + view;
        }

        view = view.Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
    }

    if (Application::cursor_newypos != Application::cursor_ypos) {
        double diff_ypos = Application::cursor_ypos - Application::cursor_newypos;
        Application::cursor_ypos = Application::cursor_newypos;

        Vector3 view = (target - position).Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation((float)(diff_ypos * ROTSPEED * dt), right.x, right.y, right.z);
        view = rotation * view;
        if (view.y < 0.5f && view.y > 0.f) {
            target = position + view;
        }
    }
}

void Camera3::FlightModeMoveCamera(double dt, Player& player, Enemy& enemy)
{
    if (Application::cursor_newxpos != Application::cursor_xpos) {
        double diff_xpos = Application::cursor_xpos - Application::cursor_newxpos;
        Application::cursor_xpos = Application::cursor_newxpos;

        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation((float)(diff_xpos * ROTSPEED * dt), 0, 1, 0);
        Vector3 view = (target - position).Normalized();
        view = rotation * view;
        target = position + view;

        view = view.Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
    }

    if (Application::cursor_newypos != Application::cursor_ypos) {
        double diff_ypos = Application::cursor_ypos - Application::cursor_newypos;
        Application::cursor_ypos = Application::cursor_newypos;

        Vector3 view = (target - position).Normalized();
        Vector3 right = view.Cross(up);
        right.y = 0;
        right.Normalize();
        up = right.Cross(view).Normalized();
        Mtx44 rotation;
        rotation.SetToIdentity();
        rotation.SetToRotation((float)(diff_ypos * ROTSPEED * dt), right.x, right.y, right.z);
        view = rotation * view;
        target = position + view;
    }

    //change player's pitch and yaw
    Vector3 view = (target - position).Normalized();
    float scalar = view.Dot(Vector3(0, 1, 0));
    player.pitch = 110.f - Math::RadianToDegree(acos(scalar));
    player.pitch *= -1;

    view = target - position;
    view.y = 0;
    view = view.Normalized();
    player.pos = target + 10 * view;
    player.pos.y -= 6.5f;
    float scalar2 = view.Dot(Vector3(0, 0, 1));
    player.yaw = Math::RadianToDegree(acos(scalar2));
    if (view.x <= 0) {
        player.yaw = 360.f - player.yaw;
    }

    //change enemy's yaw
    Vector3 enemyView = target - enemy.pos;
    enemyView.y = 0;
    enemyView = enemyView.Normalized();
    float scalar3 = enemyView.Dot(Vector3(0, 0, 1));
    enemy.yaw = Math::RadianToDegree(acos(scalar3));
    if (enemyView.x <= 0) {
        enemy.yaw = 360.f - enemy.yaw;
    }
}

void Camera3::FlightModeUpdate(double dt, const std::vector<WallCollision>& collisionWall, const std::vector<FlightCollision>& flightCollision, const Hitbox& enemyHitbox)
{
    if (Application::IsKeyPressed('W')) {   //move forward
        Vector3 view = FLYSPEED * dt * (target - position).Normalized();
        target += view;
        position += view;

        //collision with walls
        for (unsigned i = 0; i < collisionWall.size(); ++i) {
            if (collisionWall[i].dir == WALL_LEFT) {
                if (position.x < collisionWall[i].coordinate) {
                    target -= view;
                    position -= view;
                }
            }
            else if (collisionWall[i].dir == WALL_RIGHT) {
                if (position.x > collisionWall[i].coordinate) {
                    target -= view;
                    position -= view;
                }
            }
            else if (collisionWall[i].dir == WALL_FRONT) {
                if (position.z < collisionWall[i].coordinate) {
                    target -= view;
                    position -= view;
                }
            }
            else if (collisionWall[i].dir == WALL_BACK) {
                if (position.z > collisionWall[i].coordinate) {
                    target -= view;
                    position -= view;
                }
            }
            else if (collisionWall[i].dir == WALL_TOP) {
                if (position.y > collisionWall[i].coordinate) {
                    target -= view;
                    position -= view;
                }
            }
            else if (collisionWall[i].dir == WALL_BOTTOM) {
                if (position.y < collisionWall[i].coordinate) {
                    target -= view;
                    position -= view;
                }
            }
        }

        //collision with objects
        for (unsigned i = 0; i < flightCollision.size(); ++i) {
            if (position.x > flightCollision[i].minX && position.x < flightCollision[i].maxX &&
                position.y > flightCollision[i].minY && position.y < flightCollision[i].maxY &&
                position.z > flightCollision[i].minZ && position.z < flightCollision[i].maxZ)
            {
                target -= view;
                position -= view;
            }
        }

        //collision with enemy
        if (position.x > enemyHitbox.minX && position.x < enemyHitbox.maxX &&
            position.y > enemyHitbox.minY && position.y < enemyHitbox.maxY &&
            position.z > enemyHitbox.minZ && position.z < enemyHitbox.maxZ)
        {
            target -= view;
            position -= view;
        }
    }

    if (Application::IsKeyPressed('S')) {   //move backwards
        Vector3 view = FLYSPEED * dt * (target - position).Normalized();
        target -= view;
        position -= view;

        //collision with walls
        for (unsigned i = 0; i < collisionWall.size(); ++i) {
            if (collisionWall[i].dir == WALL_LEFT) {
                if (position.x < collisionWall[i].coordinate) {
                    target += view;
                    position += view;
                }
            }
            else if (collisionWall[i].dir == WALL_RIGHT) {
                if (position.x > collisionWall[i].coordinate) {
                    target += view;
                    position += view;
                }
            }
            else if (collisionWall[i].dir == WALL_FRONT) {
                if (position.z < collisionWall[i].coordinate) {
                    target += view;
                    position += view;
                }
            }
            else if (collisionWall[i].dir == WALL_BACK) {
                if (position.z > collisionWall[i].coordinate) {
                    target += view;
                    position += view;
                }
            }
            else if (collisionWall[i].dir == WALL_TOP) {
                if (position.y > collisionWall[i].coordinate) {
                    target += view;
                    position += view;
                }
            }
            else if (collisionWall[i].dir == WALL_BOTTOM) {
                if (position.y < collisionWall[i].coordinate) {
                    target += view;
                    position += view;
                }
            }
        }

        //collision with objects
        for (unsigned i = 0; i < flightCollision.size(); ++i) {
            if (position.x > flightCollision[i].minX && position.x < flightCollision[i].maxX &&
                position.y > flightCollision[i].minY && position.y < flightCollision[i].maxY &&
                position.z > flightCollision[i].minZ && position.z < flightCollision[i].maxZ)
            {
                target += view;
                position += view;
            }
        }

        //collision with enemy
        if (position.x > enemyHitbox.minX && position.x < enemyHitbox.maxX &&
            position.y > enemyHitbox.minY && position.y < enemyHitbox.maxY &&
            position.z > enemyHitbox.minZ && position.z < enemyHitbox.maxZ)
        {
            target += view;
            position += view;
        }
    }

    if (Application::IsKeyPressed('A')) {   //strafe left
        Vector3 view = (target - position).Normalized();
        Vector3 right = FLYSPEED * dt * view.Cross(up).Normalized();
        position -= right;
        target -= right;

        //collision with walls
        for (unsigned i = 0; i < collisionWall.size(); ++i) {
            if (collisionWall[i].dir == WALL_LEFT) {
                if (position.x < collisionWall[i].coordinate) {
                    target += right;
                    position += right;
                }
            }
            else if (collisionWall[i].dir == WALL_RIGHT) {
                if (position.x > collisionWall[i].coordinate) {
                    target += right;
                    position += right;
                }
            }
            else if (collisionWall[i].dir == WALL_FRONT) {
                if (position.z < collisionWall[i].coordinate) {
                    target += right;
                    position += right;
                }
            }
            else if (collisionWall[i].dir == WALL_BACK) {
                if (position.z > collisionWall[i].coordinate) {
                    target += right;
                    position += right;
                }
            }
            else if (collisionWall[i].dir == WALL_TOP) {
                if (position.y > collisionWall[i].coordinate) {
                    target += view;
                    position += view;
                }
            }
            else if (collisionWall[i].dir == WALL_BOTTOM) {
                if (position.y < collisionWall[i].coordinate) {
                    target += right;
                    position += right;
                }
            }
        }

        //collision with objects
        for (unsigned i = 0; i < flightCollision.size(); ++i) {
            if (position.x > flightCollision[i].minX && position.x < flightCollision[i].maxX &&
                position.y > flightCollision[i].minY && position.y < flightCollision[i].maxY &&
                position.z > flightCollision[i].minZ && position.z < flightCollision[i].maxZ)
            {
                target += right;
                position += right;
            }
        }

        //collision with enemy
        if (position.x > enemyHitbox.minX && position.x < enemyHitbox.maxX &&
            position.y > enemyHitbox.minY && position.y < enemyHitbox.maxY &&
            position.z > enemyHitbox.minZ && position.z < enemyHitbox.maxZ)
        {
            target += right;
            position += right;
        }
    }

    if (Application::IsKeyPressed('D')) {   //strafe right
        Vector3 view = (target - position).Normalized();
        Vector3 right = FLYSPEED * dt * view.Cross(up).Normalized();
        position += right;
        target += right;

        //collision with walls
        for (unsigned i = 0; i < collisionWall.size(); ++i) {
            if (collisionWall[i].dir == WALL_LEFT) {
                if (position.x < collisionWall[i].coordinate) {
                    target -= right;
                    position -= right;
                }
            }
            else if (collisionWall[i].dir == WALL_RIGHT) {
                if (position.x > collisionWall[i].coordinate) {
                    target -= right;
                    position -= right;
                }
            }
            else if (collisionWall[i].dir == WALL_FRONT) {
                if (position.z < collisionWall[i].coordinate) {
                    target -= right;
                    position -= right;
                }
            }
            else if (collisionWall[i].dir == WALL_BACK) {
                if (position.z > collisionWall[i].coordinate) {
                    target -= right;
                    position -= right;
                }
            }
            else if (collisionWall[i].dir == WALL_TOP) {
                if (position.y > collisionWall[i].coordinate) {
                    target -= view;
                    position -= view;
                }
            }
            else if (collisionWall[i].dir == WALL_BOTTOM) {
                if (position.y < collisionWall[i].coordinate) {
                    target -= right;
                    position -= right;
                }
            }
        }

        //collision with objects
        for (unsigned i = 0; i < flightCollision.size(); ++i) {
            if (position.x > flightCollision[i].minX && position.x < flightCollision[i].maxX &&
                position.y > flightCollision[i].minY && position.y < flightCollision[i].maxY &&
                position.z > flightCollision[i].minZ && position.z < flightCollision[i].maxZ)
            {
                target -= right;
                position -= right;
            }
        }

        //collision with enemy
        if (position.x > enemyHitbox.minX && position.x < enemyHitbox.maxX &&
            position.y > enemyHitbox.minY && position.y < enemyHitbox.maxY &&
            position.z > enemyHitbox.minZ && position.z < enemyHitbox.maxZ)
        {
            target -= right;
            position -= right;
        }
    }

}