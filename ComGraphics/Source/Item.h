/******************************************************************************/
/*!
\file	Item.h
\author 3000 Lines Studio
\brief
Item header file
*/
/******************************************************************************/

#include <string>

#ifndef ITEM_H
#define ITEM_H

/******************************************************************************/
/*!
Class Item:
\brief	Contains the information for an item
*/
/******************************************************************************/
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

/******************************************************************************/
/*!
Class Gift:
\brief	Contains the information for gifts
*/
/******************************************************************************/
class Gift : public Item
{
private:
	float value_;   //how much the item costs

public:
	Gift();
    Gift(std::string name, int value);
	~Gift();
	float getValue();
};

#endif