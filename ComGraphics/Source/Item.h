#include <string>

#ifndef ITEM_H
#define ITEM_H

class Item
{
private:
    Item();
    virtual ~Item();
    std::string name_;

public:
    std::string getName();
};

#endif