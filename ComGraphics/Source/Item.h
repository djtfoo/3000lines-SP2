#include <string>

#ifndef ITEM_H
#define ITEM_H

class Item
{
private:
    std::string name_;

protected:
    Item();
	Item(std::string name);

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
	Gift(std::string name);
	~Gift();
	float getValue();
};

#endif