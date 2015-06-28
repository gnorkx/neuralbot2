#include "food.h"

food::food(Coord Pos, float Life)
{
    Pos_= Pos;
    life_=Life;
    size_ = 1;
    if(life_>0) c_={255,0,0};
    else        c_={255,0,255};

    active_=1;
}

food::~food()
{
    //dtor
}

void food::update()
{
    if(active_)
        renderer::Instance()->Circle(Pos_,size_, c_);
    else
        {
            active_=inactiveCounter_++/500;
            if(active_)
                subject_.notify(this,stats::event::active);
        }
}

void food::SetInactive()
{
    active_=0;
    inactiveCounter_=0;
    subject_.notify(this,stats::event::inactive);
}
