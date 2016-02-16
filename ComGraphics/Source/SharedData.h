#ifndef SHAREDDATA_H
#define SHAREDDATA_H

class SharedData
{
public:
    SharedData* GetInstance()
    {
        static SharedData data;
        return &data;
    }

private:
    SharedData();
    //Player *player;
};

#endif /*SHAREDDATA_H*/