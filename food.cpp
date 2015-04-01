#include "food.h"

food::food(Coord Pos, float Life)
{
    Pos_= Pos;
    life_=Life;
    size_ = 1;
    if(life_>0) c_={255,0,0};
    else        c_={255,0,255};
}

food::~food()
{
    //dtor
}

void food::update()
{
    renderer::Instance()->Circle(Pos_,size_, c_);
}
