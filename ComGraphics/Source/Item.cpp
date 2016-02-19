#include "Item.h"

Item::Item() : name_("")
{
}

Item::~Item()
{
}

std::string Item::getName()
{
	return name_;
}

Gift::Gift() : value_(0)
{

}

Gift::~Gift()
{
}
	
float Gift::getValue()
{
	return value_;
}