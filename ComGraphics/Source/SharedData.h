#include "Character.h"

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

private:
    SharedData() {
        player = new Player("name");
    }
    ~SharedData() {
        delete player;
    }
};

#endif /*SHAREDDATA_H*/