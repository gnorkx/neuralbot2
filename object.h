#ifndef _OBJECT_H
#define _OBJECT_H

#include"color.h"
#include"Coord.h"
#include"renderer.h"

class object
{
  public:
    Coord Pos_;
    Coord Direction_;

    float size_;
    color c_;

    int active_;

    virtual void update() = 0;
    virtual ~object(){};
};


#endif
