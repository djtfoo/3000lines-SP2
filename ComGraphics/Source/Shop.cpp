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
}

Shop::~Shop()
{
}

void Shop::CheckCursor(double dt, int value)
{
    elapsedTime += dt;

    if (Application::IsKeyPressed(VK_LBUTTON) && elapsedTime > bufferTime) {
        
        //toggle left
        if (SharedData::GetInstance()->cursor_newxpos >= (SharedData::GetInstance()->width * 0.271f) && SharedData::GetInstance()->cursor_newxpos <= (SharedData::GetInstance()->width * 0.364f) &&
            SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height * 0.444f) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height * 0.574f))
        {
            if (shopIterator == ShopItemsID.begin()) {  //reached beginning of list
                shopIterator = ShopItemsID.end();
            }
            --shopIterator;

            elapsedTime = 0;
        }

        //toggle right
        if (SharedData::GetInstance()->cursor_newxpos >= (SharedData::GetInstance()->width * 0.646f) && SharedData::GetInstance()->cursor_newxpos <= (SharedData::GetInstance()->width * 0.740f) &&
            SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height * 0.444f) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height * 0.574f))
        {
            ++shopIterator;
            if (shopIterator == ShopItemsID.end()) {    //reached end of list
                shopIterator = ShopItemsID.begin();
            }
            
            elapsedTime = 0;
        }

        //buy button
        if (SharedData::GetInstance()->player->getGold() >= value) {
            if (SharedData::GetInstance()->cursor_newxpos >= (SharedData::GetInstance()->width * 0.755f) && SharedData::GetInstance()->cursor_newxpos <= SharedData::GetInstance()->width &&
                SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 45.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 40.5f)))
            {
				if (SharedData::GetInstance()->player->invfull() == false)
				{
					SharedData::GetInstance()->player->changeGold(-value);
					SharedData::GetInstance()->player->addItem(*shopIterator);
					elapsedTime = 0;
				}
				else
				{
					SharedData::GetInstance()->player->invfulldisplay = true;
				}
            }
        }

        //exit button
        if (SharedData::GetInstance()->cursor_newxpos >= (SharedData::GetInstance()->width * 0.755f) && SharedData::GetInstance()->cursor_newxpos <= SharedData::GetInstance()->width &&
            SharedData::GetInstance()->cursor_newypos >= (SharedData::GetInstance()->height / 60 * (60 - 24.5f)) && SharedData::GetInstance()->cursor_newypos <= (SharedData::GetInstance()->height / 60 * (60 - 19.5f)))
        {
            SharedData::GetInstance()->gamestate = GAME_STATE_FREE;
            elapsedTime = 0;
			SharedData::GetInstance()->player->invfulldisplay = false;
        }
    }
}