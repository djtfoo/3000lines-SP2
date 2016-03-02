#include "Pause.h"
#include "Application.h"
#include "SharedData.h"

Pause::Pause() : elapsedTime(0), bufferTime(0.125), maxDisp1(38), maxDisp2(28), maxDisp3(18)
{
    PauseReset();
    b_ResumeGame.Set(0.32f, 0.695f, 0.338f, 0.421f);
    b_ExitToMainMenu.Set(0.32f, 0.695f, 0.505f, 0.588f);
    b_ExitGame.Set(0.32f, 0.695f, 0.671f, 0.755f);
}

Pause::~Pause()
{
}

void Pause::PauseReset()
{
    verticalDisp1 = 70;
    verticalDisp2 = 80;
    verticalDisp3 = 90;
    animation = true;
}

void Pause::PauseAnimation(double dt)
{
    if (verticalDisp1 > maxDisp1) {
        verticalDisp1 -= 80 * dt;
    }
    if (verticalDisp2 > maxDisp2) {
        verticalDisp2 -= 80 * dt;
    }
    if (verticalDisp3 > maxDisp3) {
        verticalDisp3 -= 80 * dt;
    }

    if (verticalDisp1 < maxDisp1) {
        verticalDisp1 = maxDisp1;
    }
    if (verticalDisp2 < maxDisp2) {
        verticalDisp2 = maxDisp2;
    }
    if (verticalDisp3 < maxDisp3) {
        verticalDisp3 = maxDisp3;
    }

    if (verticalDisp1 == maxDisp1 && verticalDisp2 == maxDisp2 && verticalDisp3 == maxDisp3) {    //buttons finished falling
        animation = false;
    }
}

void Pause::CheckCursor(double dt)
{
    elapsedTime += dt;

    if (Application::IsKeyPressed(VK_LBUTTON) && elapsedTime > bufferTime) {

        if (SharedData::GetInstance()->cursor_newxpos >= b_ResumeGame.minX && SharedData::GetInstance()->cursor_newxpos <= b_ResumeGame.maxX) {
            //resume game
            if (SharedData::GetInstance()->cursor_newypos >= b_ResumeGame.minY && SharedData::GetInstance()->cursor_newypos <= b_ResumeGame.maxY)
            {
                SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
                SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;
                SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;
                PauseReset();
                elapsedTime = 0;
            }

            //exit to main menu
            else if (SharedData::GetInstance()->cursor_newypos >= b_ExitToMainMenu.minY && SharedData::GetInstance()->cursor_newypos <= b_ExitToMainMenu.maxY)
            {
                SharedData::GetInstance()->programstate_change = true;
                SharedData::GetInstance()->program_state = PROGRAM_MENU;
                SharedData::GetInstance()->gamestate = GAME_STATE_FREE;     //reset game state
                PauseReset();
                elapsedTime = 0;
            }

            //exit program
            else if (SharedData::GetInstance()->cursor_newypos >= b_ExitGame.minY && SharedData::GetInstance()->cursor_newypos <= b_ExitGame.maxY)
            {
                SharedData::GetInstance()->program_state = PROGRAM_EXIT;
            }
        }

    }
}