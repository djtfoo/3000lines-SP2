/******************************************************************************/
/*!
\file	Application.h
\author 3000 Lines Studio
\brief
Application header file
*/
/******************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

/******************************************************************************/
/*!
Class Application:
\brief	A class for the application
*/
/******************************************************************************/
class Application
{

public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

private:
    void get_resolution();

	//Declare a window object
	StopWatch m_timer;
};

#endif