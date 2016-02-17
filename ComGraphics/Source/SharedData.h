#include "Character.h"
#include "Camera3.h"

#ifndef SHAREDDATA_H
#define SHAREDDATA_H

class SharedData
{
public:
    static SharedData* GetInstance()
    {
        static SharedData data;
        return &data;
    }

    Player *player;
    Camera3 *camera;

    //cursor
    double cursor_xpos;
    double cursor_ypos;
    double cursor_newxpos;
    double cursor_newypos;

private:
    SharedData() {
        player = new Player("Player");
        camera = new Camera3();
        camera->Init(Vector3(0, -140, 100), Vector3(0, -140, 110), Vector3(0, 1, 0));

        cursor_xpos = 400;
        cursor_ypos = 300;
        cursor_newxpos = 400;
        cursor_newypos = 300;
    }
    ~SharedData() {
        delete player;
        delete camera;
    }
};

#endif /*SHAREDDATA_H*/