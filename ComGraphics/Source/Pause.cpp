#include "Pause.h"
#include "Application.h"
#include "SharedData.h"

Pause::Pause() : elapsedTime(0), bufferTime(0.125), maxDisp1(38), maxDisp2(28), maxDisp3(18)
{
    PauseReset();
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

        if (SharedData::GetInstance()->cursor_newxpos >= (SharedData::GetInstance()->width * 0.32f) && SharedData::GetInstance()->cursor_newxpos <= (SharedData::GetInstance()->width * 0.695f)) {
            //resume game
            if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height * 0.338f) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height * 0.421f))
            {
                SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
                SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;
                SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;
                PauseReset();
                elapsedTime = 0;
            }

            //exit to main menu
            else if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height * 0.505f) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height * 0.588f))
            {
                SharedData::GetInstance()->programstate_change = true;
                SharedData::GetInstance()->program_state = PROGRAM_MENU;
                SharedData::GetInstance()->gamestate = GAME_STATE_FREE;     //reset game state
                PauseReset();
                elapsedTime = 0;
            }

            //exit program
            else if (SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height * 0.671f) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height * 0.755f))
            {
                SharedData::GetInstance()->program_state = PROGRAM_EXIT;
            }
        }

    }
}