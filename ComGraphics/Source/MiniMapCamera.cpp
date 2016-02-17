#include "MiniMapCamera.h"

#include "MyMath.h"
#include "Application.h"
#include "Mtx44.h"

#include "SharedData.h"

Minimapcamera::Minimapcamera() : phi(15), theta(20), dist(25)
{

}
Minimapcamera::~Minimapcamera()
{

}

void Minimapcamera::Init(const Vector3& position, const Vector3& target, const Vector3& up)
{
    this->pos = defaultPos = position;
    this->targ = defaultTar = target;
    Vector3 view = (target - pos).Normalized();
    Vector3 right = view.Cross(up);
    right.y = 0;
    right.Normalize();
    this->UP = defaultUP = right.Cross(view).Normalized();
}

void Minimapcamera::Update(double dt)
{
    Vector3 playerPos = (SharedData::GetInstance()->player->position_);

    //Camera Position Follows Player
    this->pos.x = playerPos.x;
    this->pos.y = (playerPos.y + 100);
    this->pos.z = playerPos.z;

    //this->targ = (0, -1, 0); //Later Initialize



}