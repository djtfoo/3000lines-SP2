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
    bool checkPuzzleAns(int first, int second, int third, int forth);
    //void puzzleChecker(int first, int second, int third, int forth);
    float puzzleSize[4][4];
    float puzzleAns[4][4];

    bool one, two, three, four;

    int checker;

    Vector3 puzzlePos;

private:
    int puzzlechecked[4][4];
};

#endif /* LIGHTPUZZLE_H */