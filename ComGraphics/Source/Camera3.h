#include "Vector3.h"

#include "Character.h"
#include "Collision.h"

#include <vector>

#ifndef CAMERA_3_H
#define CAMERA_3_H

class Camera3
{
private:
    Vector3 defaultPosition;
    Vector3 defaultTarget;
    Vector3 defaultUp;

public:
    Vector3 position;
    Vector3 target;
    Vector3 up;

    Camera3();
    ~Camera3();
    void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    void Init(const float dist, const Vector3& pos, const Vector3& target, const Vector3& up);
    void Reset();       //reset, unlocking camera target on model
    void Update(double dt);
    void Update(double dt, const std::vector<WallCollision>& collisionWall, const std::vector<ItemCollision>& collisionItem);
    void Update(double dt, Player& player, const std::vector<WallCollision>& collisionWall, const std::vector<ItemCollision>& collisionItem, const StairsCollision& stairs);
    void MoveCamera(double dt);

    void LimitedMoveCamera(double dt);

    void MoveCamera(double dt, Player& player);
    void FlightModeUpdate(double dt, const std::vector<WallCollision>& collisionWall, const std::vector<FlightCollision>& flightCollision, const Hitbox& enemyHitbox);
};

#endif