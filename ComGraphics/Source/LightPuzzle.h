/******************************************************************************/
/*!
\file	LightPuzzle.h
\author 3000 Lines Studio
\brief
LightPuzzle header file
*/
/******************************************************************************/

#ifndef LIGHTPUZZLE_H
#define LIGHTPUZZLE_H

#include "Vector3.h"
#include <cstdlib>

/******************************************************************************/
/*!
Class LightPuzzle:
\brief	Contains the information for the puzzle
*/
/******************************************************************************/
class LightPuzzle
{
public:
    LightPuzzle();
    ~LightPuzzle();

    void generatePuzzle();
    bool checkPuzzleAns(int first, int second, int third, int forth);
    float puzzleSize[4];
    float puzzleAns[4];

    bool one, two, three, four;
    int checker;
};

#endif /* LIGHTPUZZLE_H */