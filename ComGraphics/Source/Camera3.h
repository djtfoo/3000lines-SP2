#include "Vector3.h"

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
    float phi;          //this is the angle of elevation
    float theta;        //this is the angle of azimuth
    float distance;     //this is the distance between camera and the origin

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