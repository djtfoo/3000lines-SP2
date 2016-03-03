/******************************************************************************/
/*!
\file	Pause.h
\author 3000 Lines Studio
\brief
Pause header file
*/
/******************************************************************************/

#include "Button.h"

#ifndef PAUSE_H
#define PAUSE_H

/******************************************************************************/
/*!
Class Pause:
\brief	Stores information for pause menu
*/
/******************************************************************************/
class Pause
{
private:
    double elapsedTime;
    double bufferTime;      //prevent player from clicking at the beginning of pause screen

    void PauseReset();      //reset the pause buttons' y-coordinate positions
    float maxDisp1, maxDisp2, maxDisp3;     //the final y-coordinate position of each button in the pause screen

    Button b_ResumeGame;
    Button b_ExitToMainMenu;
    Button b_ExitGame;

public:
    bool animation;
    float verticalDisp1, verticalDisp2, verticalDisp3;  //the y-coordinate position of each button in the pause screen

    Pause();
    ~Pause();

    void PauseAnimation(double dt);
    void CheckCursor(double dt);
};

#endif