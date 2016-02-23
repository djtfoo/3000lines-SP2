#include "LightPuzzle.h"
#include <iostream>

using std::cout;
using std::endl;

LightPuzzle::LightPuzzle() : checker(0)
{
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
bool LightPuzzle::checkPuzzle(int first, int second, int third, int forth)
{
    puzzleAns[0][0] = first;
    puzzleAns[0][1] = second;
    puzzleAns[0][2] = third;
    puzzleAns[0][3] = forth;

    for (int i = 0; i < 1; i++)
    {
        for (int p = 0; p < 4; p++)
        {
            if (puzzleAns[i][p] == puzzleSize[i][p])
            {
                checker++;
            }
        }
    }
    if (checker >= 4)
    {
        checker = 0;
        return true;
    }

return false;
}
void LightPuzzle::setPuzzle(float posX, float posY, float posZ)
{
    puzzlePos.Set(posX, posY, posZ);
}
