#include "move.h"
#include "../objects/bot.h"



move::move()
{
    //ctor
    nRepresentations_ = 4;
}

move::~move()
{
    //dtor
}


void move::perform(bot& b, int iPar, float* fPar )
{
    float f = 0.1;
    Coord vel;
    switch( iPar )
    {
        case 0:
            vel = {1,0};
            break;
        case 1:
            vel = {-1,0};
            break;
        case 2:
            vel = {0,1};
            break;
        case 3:
            vel = {0,-1};
            break;
    }

    b.Vel_+=f*vel;
}
