#include "LightPuzzle.h"
#include "SharedData.h"
#include <iostream>

using std::cout;
using std::endl;

LightPuzzle::LightPuzzle() : checker(0)
{
    one = two = three = four = false;

    puzzlePos.Set(0, 0, 0);
    for (int i = 0; i < 4; i++)
    {
        for (int p = 0; p < 4; p++)
        {
            puzzleSize[i][p] = 0.f;
        }
    }
}
LightPuzzle::~LightPuzzle()
{
    
}
void LightPuzzle::generatePuzzle()
{
    int puzzleSizeY = 4;
    for (int i = 0; i < 1; i++)
    {
        for (int p = 0; p < puzzleSizeY; p++)
        {
            puzzleSize[i][p] = rand() % 5 + 1;
            if (puzzleSize[i][p] == 1)
            {
                //code red
                cout << "red ";
            }
            if (puzzleSize[i][p] == 2)
            {
                //code blue
                cout << "blue ";
            }
            if (puzzleSize[i][p] == 3)
            {
                //code green
                cout << "green ";
            }
            if (puzzleSize[i][p] == 4)
            {
                //code Yellow
                cout << "Yellow ";
            }
            if (puzzleSize[i][p] == 5)
            {
                //code cyan
                cout << "cyan ";
            }
        }
        cout << endl;
    }
}
bool LightPuzzle::checkPuzzleAns(int first, int second, int third, int forth)
{
    puzzleAns[0][0] = first;
    puzzleAns[0][1] = second;
    puzzleAns[0][2] = third;
    puzzleAns[0][3] = forth;

    if (SharedData::GetInstance()->switchFlip == true)
    {
        if (puzzleAns[0][0] == puzzleSize[0][0])
        {
            one = true;
            SharedData::GetInstance()->switch1 = true;
        }
        else
        {
            one = false;
            SharedData::GetInstance()->switch1 = false;
        }
        if (puzzleAns[0][1] == puzzleSize[0][1])
        {
            two = true;
            SharedData::GetInstance()->switch2 = true;
        }
        else
        {
            two = false;
            SharedData::GetInstance()->switch2 = false;
        }
        if (puzzleAns[0][2] == puzzleSize[0][2])
        {
            three = true;
            SharedData::GetInstance()->switch3 = true;
        }
        else
        {
            three = false;
            SharedData::GetInstance()->switch3 = false;
        }
        if (puzzleAns[0][3] == puzzleSize[0][3])
        {
            four = true;
            SharedData::GetInstance()->switch4 = true;
        }
        else
        {
            four = false;
            SharedData::GetInstance()->switch4 = false;
        }
        if (one == true && two == true && three == true && four == true)
        {
            one = two = three = four = false;
            SharedData::GetInstance()->switch1 = SharedData::GetInstance()->switch2 = SharedData::GetInstance()->switch3 = SharedData::GetInstance()->switch4 = false;
            return true;
        }
        SharedData::GetInstance()->switchFlip = false;
    }

return false;
}