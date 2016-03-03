/******************************************************************************/
/*!
\file	MiniMapCamera.cpp
\author 3000 Lines Studio
\brief
Functions for the minimap
*/
/******************************************************************************/

#include "MiniMapCamera.h"

#include "MyMath.h"
#include "Application.h"
#include "Mtx44.h"

#include "SharedData.h"

/******************************************************************************/
/*!
\brief	Minimapcamera default constructor
*/
/******************************************************************************/
Minimapcamera::Minimapcamera() : phi(15), theta(20), dist(25)
{

}

/******************************************************************************/
/*!
\brief	Minimapcamera default destructor
*/
/******************************************************************************/
Minimapcamera::~Minimapcamera()
{

}
/******************************************************************************/
/*!
\brief	Minimapcamera minimap

\param position
    Position of Camera
\param target
    Target of the camera
\param up
    UP vector of the camera
\return
    None
*/
/******************************************************************************/
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
/******************************************************************************/
/*!
\brief	Mini map camera update

\param dt
    Elapsed Times
*/
/******************************************************************************/
void Minimapcamera::Update(double dt)
{

}