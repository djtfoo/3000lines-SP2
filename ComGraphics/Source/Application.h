
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{

public:
	int width;
	int height;

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