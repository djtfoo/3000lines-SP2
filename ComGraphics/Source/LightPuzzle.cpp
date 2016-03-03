#include "LightPuzzle.h"
#include "SharedData.h"
#include <iostream>

using std::cout;
using std::endl;

LightPuzzle::LightPuzzle() : checker(0)
{
    one = two = three = four = false;
    for (int i = 0; i < 4; i++)
    {
        puzzleSize[i] = 0.f;
    }
}
LightPuzzle::~LightPuzzle()
{
    
}
void LightPuzzle::generatePuzzle()
{
    int puzzleSizeY = 4;
    for (int p = 0; p < puzzleSizeY; p++)
    {
        puzzleSize[p] = rand() % 6 + 1;  //1 to 6
        if (puzzleSize[p] == 1)
        {
            cout << "red ";
        }
        if (puzzleSize[p] == 2)
        {
            cout << "blue ";
        }
        if (puzzleSize[p] == 3)
        {
            cout << "green ";
        }
        if (puzzleSize[p] == 4)
        {
            cout << "cyan ";
        }
        if (puzzleSize[p] == 5)
        {
            cout << "yellow ";
        }
        if (puzzleSize[p] == 6)
        {
            cout << "pink ";
        }
    }

    SharedData::GetInstance()->firstball = puzzleSize[0];   //Chon ball game here XD stealing rand variable
}
bool LightPuzzle::checkPuzzleAns(int first, int second, int third, int forth)
{
    puzzleAns[0] = first; //green
    puzzleAns[1] = second;
    puzzleAns[2] = third;
    puzzleAns[3] = forth;

    if (SharedData::GetInstance()->switchFlip == true)
    {
        //Old stuff
        if (puzzleAns[0] == puzzleSize[0])
        {
            one = true;
            SharedData::GetInstance()->switch1 = true;
        }
        else
        {
            one = false;
            SharedData::GetInstance()->switch1 = false;
        }
        if (puzzleAns[1] == puzzleSize[1])
        {
            two = true;
            SharedData::GetInstance()->switch2 = true;
        }
        else
        {
            two = false;
            SharedData::GetInstance()->switch2 = false;
        }
        if (puzzleAns[2] == puzzleSize[2])
        {
            three = true;
            SharedData::GetInstance()->switch3 = true;
        }
        else
        {
            three = false;
            SharedData::GetInstance()->switch3 = false;
        }
        if (puzzleAns[3] == puzzleSize[3])
        {
            four = true;
            SharedData::GetInstance()->switch4 = true;
        }
        else
        {
            four = false;
            SharedData::GetInstance()->switch4 = false;
        }

        //game cleared
        if (one && two && three && four)
        {
            SharedData::GetInstance()->switchCount = 0;
            one = two = three = four = false;
            SharedData::GetInstance()->switch1 = SharedData::GetInstance()->switch2 = SharedData::GetInstance()->switch3 = SharedData::GetInstance()->switch4 = false;
            return true;
        }
        SharedData::GetInstance()->switchFlip = false;
    }

    return false;
}