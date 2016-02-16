
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{

public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

    //cursor
    static double cursor_xpos;
    static double cursor_ypos;
    static double cursor_newxpos;
    static double cursor_newypos;

private:

	//Declare a window object
	StopWatch m_timer;
};

#endif