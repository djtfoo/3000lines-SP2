#include "Shop.h"
#include "Application.h"
#include "SharedData.h"

Shop::Shop() : elapsedTime(0), bufferTime(0.125)
{
    ShopItemsID.push_back(1);
    for (int i = 6; i <= 13; ++i) {
        ShopItemsID.push_back(i);
    }
    shopIterator = ShopItemsID.begin();

    b_ToggleLeft.Set(0.271f, 0.364f, 0.444f, 0.574f);
    b_ToggleRight.Set(0.646f, 0.74f, 0.444f, 0.574f);
    b_Buy.Set(0.755f, 1.f, 0.242f, 0.325f);
    b_Exit.Set(0.755f, 1.f, 0.592f, 0.675f);
}

Shop::~Shop()
{
}

void Shop::CheckCursor(double dt, int value)
{
    elapsedTime += dt;

    if (Application::IsKeyPressed(VK_LBUTTON) && elapsedTime > bufferTime) {
        
        //toggle left
        if (SharedData::GetInstance()->cursor_newxpos >= b_ToggleLeft.minX && SharedData::GetInstance()->cursor_newxpos <= b_ToggleLeft.maxX &&
            SharedData::GetInstance()->cursor_newypos >= b_ToggleLeft.minY && SharedData::GetInstance()->cursor_newypos <= b_ToggleLeft.maxY)
        {
            if (shopIterator == ShopItemsID.begin()) {  //reached beginning of list
                shopIterator = ShopItemsID.end();
            }
            --shopIterator;

            elapsedTime = 0;
        }

        //toggle right
        else if (SharedData::GetInstance()->cursor_newxpos >= b_ToggleRight.minX && SharedData::GetInstance()->cursor_newxpos <= b_ToggleRight.maxX &&
            SharedData::GetInstance()->cursor_newypos >= b_ToggleRight.minY && SharedData::GetInstance()->cursor_newypos <= b_ToggleRight.maxY)
        {
            ++shopIterator;
            if (shopIterator == ShopItemsID.end()) {    //reached end of list
                shopIterator = ShopItemsID.begin();
            }
            
            elapsedTime = 0;
        }

        //buy button
        if (SharedData::GetInstance()->cursor_newxpos >= b_Buy.minX && SharedData::GetInstance()->cursor_newxpos <= b_Buy.maxX &&
            SharedData::GetInstance()->cursor_newypos >= b_Buy.minY && SharedData::GetInstance()->cursor_newypos <= b_Buy.maxY)
        {
            if (SharedData::GetInstance()->player->getGold() >= value) {
                if (!SharedData::GetInstance()->player->invfull())      //if player's inventory is not full
                {
                    SharedData::GetInstance()->player->changeGold(-value);
                    SharedData::GetInstance()->player->addItem(*shopIterator);
                    elapsedTime = 0;
                }
                else
                {
                    SharedData::GetInstance()->player->invfulldisplay = true;   //tell the player that inventory is full
                }
            }
        }

        //exit button
        else if (SharedData::GetInstance()->cursor_newxpos >= b_Exit.minX && SharedData::GetInstance()->cursor_newxpos <= b_Exit.maxX &&
            SharedData::GetInstance()->cursor_newypos >= b_Exit.minY && SharedData::GetInstance()->cursor_newypos <= b_Exit.maxY)
        {
            SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
            SharedData::GetInstance()->cursor_xpos = SharedData::GetInstance()->cursor_newxpos;
            SharedData::GetInstance()->cursor_ypos = SharedData::GetInstance()->cursor_newypos;

            //resetting the shop
            shopIterator = ShopItemsID.begin();
            elapsedTime = 0;
			SharedData::GetInstance()->player->invfulldisplay = false;
        }
    }
}