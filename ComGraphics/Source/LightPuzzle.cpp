/******************************************************************************/
/*!
\file	LightPuzzle.cpp
\author 3000 Lines Studio
\brief
Logic for light puzzle
*/
/******************************************************************************/

#include "LightPuzzle.h"
#include "SharedData.h"

/******************************************************************************/
/*!
\brief	LightPuzzle default constructor
*/
/******************************************************************************/
LightPuzzle::LightPuzzle() : checker(0)
{
    one = two = three = four = false;
    for (int i = 0; i < 4; i++)
    {
        puzzleSize[i] = 0.f;
    }
}

/******************************************************************************/
/*!
\brief	LightPuzzle default destructor
*/
/******************************************************************************/
LightPuzzle::~LightPuzzle()
{
    
}
/******************************************************************************/
/*!
\brief	Generate Light Puzzle
    In an array and radomized puzzle
\return
    None
*/
/******************************************************************************/
void LightPuzzle::generatePuzzle()
{
    int puzzleSizeY = 4;
    for (int p = 0; p < puzzleSizeY; p++)
    {
        puzzleSize[p] = rand() % 6 + 1;  //1 to 6
    }

    SharedData::GetInstance()->firstball = puzzleSize[0];   //Chon ball game here XD stealing rand variable
}
/******************************************************************************/
/*!
\brief	Checking Puzzle Answer.
    Input 4 answers and check them.
\param first
    First value of the answer.
\param second
    Second value of the answer.
\param third
    Third value of the answer.
\param forth
    Forth value of the answer.
\return
    True if all answers are correct.
*/
/******************************************************************************/
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