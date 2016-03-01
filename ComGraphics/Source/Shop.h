#include "Item.h"
#include <vector>

#ifndef SHOP_H
#define SHOP_H

class Shop
{
private:
    double elapsedTime, bufferTime;
    std::vector<int> ShopItemsID;

public:
    Shop();
    ~Shop();

    std::vector<int>::iterator shopIterator;
    void CheckCursor(double dt, int value);
};

#endif