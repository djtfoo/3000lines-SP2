#ifndef LIGHTPUZZLE_H
#define LIGHTPUZZLE_H

#include "Vector3.h"
#include <cstdlib>

class LightPuzzle
{
public:
    LightPuzzle();
    ~LightPuzzle();

    void generatePuzzle();
    void setPuzzle(float posX, float posY, float posZ);
    bool checkPuzzle(int first, int second, int third, int forth);
    float puzzleSize[4][4];
    float puzzleAns[4][4];

    int checker;

    Vector3 puzzlePos;

private:
    int puzzlechecked[4][4];
};

#endif /* LIGHTPUZZLE_H */