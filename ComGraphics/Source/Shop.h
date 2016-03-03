/******************************************************************************/
/*!
\file	Shop.h
\author 3000 Lines Studio
\brief
Shop header file
*/
/******************************************************************************/

#include "Item.h"
#include "Button.h"
#include <vector>

#ifndef SHOP_H
#define SHOP_H

/******************************************************************************/
/*!
Class Shop:
\brief	Stores all information for the shop
*/
/******************************************************************************/
class Shop
{
private:
    double elapsedTime, bufferTime;
    std::vector<int> ShopItemsID;

    Button b_ToggleLeft;
    Button b_ToggleRight;
    Button b_Buy;
    Button b_Exit;

public:
    Shop();
    ~Shop();

    std::vector<int>::iterator shopIterator;
    void CheckCursor(double dt, int value);
};

#endif