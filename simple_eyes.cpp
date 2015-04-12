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

void simple_eyes::update(const bot &bot_, int &nVal, float *&Value)
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
//  );

    if(gWorld.size()>0)
    {
        std::partial_sort(gWorld.begin(),
            gWorld.begin()+1,
            gWorld.end(),
            [=]( object *a,  object *b){ float Diff_a = a->active_?(a->Pos_- bot_.Pos_).abs():1.e20;
                                 float Diff_b = b->active_?(b->Pos_ - bot_.Pos_).abs():1.e20;
                                return Diff_a<Diff_b;}
                                );

        if(gWorld[0]->active_)
        {
            Coord Diff = gWorld[0]->Pos_ - bot_.Pos_;
            value_[0] = Diff.x;
            value_[1] = Diff.y;
        } else
            value_[0] = value_[1] = 0;
    }  else
        value_[0] = value_[1] = 0;

    nVal = nVal_;
    Value = value_;

}
