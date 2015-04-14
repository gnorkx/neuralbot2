#include "rgba_vision.h"

extern std::vector<object*> gWorld;


rgba_vision::rgba_vision()
{
    //ctor
    nVal_ = 2;
    value_ = new float[nVal_];
}

rgba_vision::~rgba_vision()
{
    //dtor
    delete[] value_;
}

void rgba_vision::update(const bot &bot_, int &nVal, float *&Value)
{

    int nPixel_ = 99;
    float openingAngle_ = 45.;

    color cArray_[nPixel_];
    for(int i = 0; i < nPixel_; i++)
        cArray_[i] = {100,100,100};


    sort(gWorld.begin(), gWorld.end(),
        [=](object *a, object *b){
            float diff_a = a->active_?(a->Pos_ - bot_.Pos_).abs():1.e20;
            float diff_b = b->active_?(b->Pos_ - bot_.Pos_).abs():1.e20;
            return diff_a>diff_b;}
            );

    for(auto it = gWorld.begin(); it != gWorld.end(); ++it)
    {
        Coord Diff = (*it)->Pos_ - bot_.Pos_;
        Coord DiffPerpUnit = (Diff.perp()).unit();
        Coord DiffA = Diff + (*it)->size_*DiffPerpUnit;
        Coord DiffB = Diff - (*it)->size_*DiffPerpUnit;
        cout<<Diff<<" -- "<<DiffA<<" -- "<<DiffB<<endl;

        float alphaA = acos(bot_.Direction_.SinTheta(DiffA))/M_PI*180. -90;
        float alphaB = acos(bot_.Direction_.SinTheta(DiffB))/M_PI*180. -90;
        cout<<alphaA<<" "<<alphaB<<endl;
        int iDiffA = alphaA/openingAngle_*nPixel_/2-nPixel_/2;
        int iDiffB = alphaB/openingAngle_*nPixel_/2-nPixel_/2;

        cout<<iDiffA<<" "<<iDiffB<<endl;

        bool fl_print = false;
        for(int i = 0; i < nPixel_; i++)
        {
            if(i == iDiffA || i==iDiffB)
                fl_print = !fl_print;

            if(fl_print)
                cArray_[i] = (*it)->c_;

        }
    }

    for(int i = 0; i < nPixel_; i++)
    {
        renderer::Instance()->Rect({i,0}, {1,1}, cArray_[i]);
    }



    Value = value_;
}

