#ifndef _OBJECT_H
#define _OBJECT_H

#include"Coord.h"

class object
{
    Coord Pos_;
    Coord Direction_;

    virtual void update() = 0;
};


#endif
