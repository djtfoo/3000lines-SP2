#include "Item.h"

Item::Item() : name_("")
{
}

Item::Item(std::string name) : name_(name)
{
}

Item::~Item()
{
}

std::string Item::getName()
{
	return name_;
}

Gift::Gift() : Item(), value_(0)
{

}

Gift::Gift(std::string name) : Item(name), value_(0)
{

}

Gift::~Gift()
{
}
	
float Gift::getValue()
{
	return value_;
}