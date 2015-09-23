#include "view.h"
#include "../objects/bot.h"
view::view()
{
    //ctor
    nRepresentations_ = 4;
}

view::~view()
{
    //dtor
}

void view::perform(bot& b, int iPar, float* fPar )
{
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
    b.Direction_=vel.unit();
}
