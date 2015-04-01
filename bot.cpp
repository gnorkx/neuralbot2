#include "bot.h"
#include "simple_eyes.h"

extern vector<object*> gWorld;

bot::bot(Coord Pos)
{
    Pos_ = Pos;
    Vel_ = {0,0};
    init();
}
//bot::bot(const bot &rhs)
//{}
//
//bot::bot(bot &&rhs)
//{}
//
bot::~bot()
{

    //delete sensors:
//    for(auto it = sensors_.begin(); it!= sensors_.end();++it)
//        delete *it;

}


void bot::update()
{

    Vel_ = {rnd0(),rnd0()};
    Pos_ += Vel_;

    int nVal;
    float *Values = nullptr;

    Coord DistClosest;
    for(auto it = sensors_.begin(); it!= sensors_.end();++it)
    {
        (*it)->update(*this, nVal, Values);
        DistClosest = {Values[0],Values[1]};
    }

    renderer::Instance()->Circle(Pos_, size_, c_);
    renderer::Instance()->Line(Pos_,Pos_+DistClosest,c_);
    cout<<Pos_<<endl;
}

void bot::init()
{

    c_ = {255,255,0};
    size_ = 1;
    sensors_.push_back(new simple_eyes);



}
