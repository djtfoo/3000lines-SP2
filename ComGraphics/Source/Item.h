#include <string>

#ifndef ITEM_H
#define ITEM_H

class Item
{
private:
    std::string name_;

protected:
    Item();

public:
    virtual ~Item();
    std::string getName();
};

#endif