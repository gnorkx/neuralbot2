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
    float openingAngle_ = M_PI*0.25;

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

        float alphaA = bot_.Direction_.ThetaMath(DiffA);
        float alphaB = bot_.Direction_.ThetaMath(DiffB);


        Coord RightFOV = bot_.Direction_; RightFOV.rotate(-openingAngle_);

        float betaA = DiffA.ThetaMath(RightFOV);
        float betaB = DiffB.ThetaMath(RightFOV);
        cout<<betaA<<" "<<betaB<<endl;

        int iPosA = betaA/(2*openingAngle_)*nPixel_;
        int iPosB = betaB/(2.*openingAngle_)*nPixel_;

        if(iPosA>nPixel_)
        {
            if(iPosB > nPixel_)
            {
                if(iPosA>iPosB) continue;
                iPosA = nPixel_-1;
                iPosB = 0;
            }
            iPosA = nPixel_-1;
        } else
        {
            if(iPosB > nPixel_)
            {
                if(iPosA>iPosB) continue;
                iPosB = 0;
            }
        }
        cout<<iPosA<< " " <<iPosB<<endl;

        for(int i = iPosB; i < iPosA; i++)
        {
                cArray_[i] = (*it)->c_;
        }
    }

    for(int i = 0; i < nPixel_; i++)
    {
        renderer::Instance()->Rect({i,0}, {1,1}, cArray_[i]);
    }



    Value = value_;
}

