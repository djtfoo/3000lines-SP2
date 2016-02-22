#include "Item.h"

#ifndef OUTFIT_H
#define OUTFIT_H

class Outfit : public Item
{
protected:
    Outfit();
    Outfit(std::string name);
    
    //texture directory
    std::string texture;

public:
    virtual ~Outfit();
};

class Hat : public Outfit
{
private:
    //obj

public:
    Hat();
    Hat(std::string name);
    virtual ~Hat();
};

class Top : public Outfit
{
public:
    Top();
    Top(std::string name);
    virtual ~Top();
};

class Bottoms : public Outfit
{
public:
    Bottoms();
    Bottoms(std::string name);
    virtual ~Bottoms();
};

#endif