#include "simple_eyes.h"

simple_eyes::simple_eyes()
{
    //ctor
    nVal_ = 2;
    value_ = new float [nVal_];
}

simple_eyes::~simple_eyes()
{
    //dtor
    delete[] value_;
}

void simple_eyes::update(bot &bot_, int &nVal, float *Value)
{

//    std::partial_sort(gWorld.begin(),
//        gWorld.size()>g_max_clobj?gWorld.begin()+g_max_clobj:gWorld.end(),
//        gWorld.end(),
//        [=](CObj &a, CObj &b){Coord Diff_a = a.Pos - this->Pos;
//                                Coord Diff_b = b.Pos - this->Pos;
//                                if(fabs(Diff_a.x)>gWorldX) Diff_a.x -= 2*signum(Diff_a.x)*gWorldX;
//                                if(fabs(Diff_a.y)>gWorldY) Diff_a.y -= 2*signum(Diff_a.y)*gWorldY;
//                                if(fabs(Diff_b.x)>gWorldX) Diff_b.x -= 2*signum(Diff_b.x)*gWorldX;
//                                if(fabs(Diff_b.y)>gWorldY) Diff_b.y -= 2*signum(Diff_b.y)*gWorldY;
//                                return Diff_a.abs()<Diff_b.abs();}
//                                );
    std::partial_sort(gWorld.begin(),
        gWorld.begin()+1,
        gWorld.end(),
        [=]( object *a,  object *b){return a->Pos_.abs()<b->Pos_.abs();});
//{ Coord Diff_a = a.Pos_ - bot_.Pos_;
//                                 Coord Diff_b = b.Pos_ - bot_.Pos_;
//                                return Diff_a.abs()<Diff_b.abs();}
//                                );

    Coord Diff = gWorld[0]->Pos_ - bot_.Pos_;
    value_[0] = Diff.x;
    value_[1] = Diff.y;
    nVal =nVal_;
    Value = value_;

}
