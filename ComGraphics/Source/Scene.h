/******************************************************************************/
/*!
\file	Scene.h
\author 3000 Lines Studio
\brief
Scene header file
*/
/******************************************************************************/

#ifndef SCENE_H
#define SCENE_H

/******************************************************************************/
/*!
Class Scene:
\brief	Stores scene information
*/
/******************************************************************************/
class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

#endif