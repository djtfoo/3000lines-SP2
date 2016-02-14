#include "Vector3.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
private:
    Vector3 defaultPosition;
    Vector3 defaultTarget;
    Vector3 defaultUp;

public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Camera();
	~Camera();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
    void Init(const float dist, const Vector3& pos, const Vector3& target, const Vector3& up);
	void Reset();       //reset, unlocking camera target on model
    void Update(double dt, const Vector3& charPos = Vector3(0, 0, 0));

    //lock camera on target
    bool targetLock;    //lock camera target on model
    void TargetLock(const Vector3& charPos);
    Vector3 move;
};

#endif