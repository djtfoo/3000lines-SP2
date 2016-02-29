#include "Item.h"

//Item abstract class
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

//Gift item
Gift::Gift() : Item()
{
}

Gift::Gift(std::string name, int value) : Item(name), value_(value)
{

}

Gift::~Gift()
{
}
	
float Gift::getValue()
{
	return value_;
}