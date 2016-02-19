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

class Gift : public Item
{
private:
	float value_;
public:
	Gift();
	~Gift();
	float getValue();
};

#endif