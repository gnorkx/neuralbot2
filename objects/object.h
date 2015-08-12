#ifndef _OBJECT_H
#define _OBJECT_H

#include"../tools/color.h"
#include"../tools/Coord.h"
#include"../tools/renderer.h"

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
