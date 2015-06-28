#include "rgba_vision.h"

extern std::vector<object*> gWorld;
extern std::vector<bot*> gBots;


rgba_vision::rgba_vision()
{
    //ctor
    nPixel_ = 50;

    nVal_ = nPixel_*3;
    openingAngle_ = 0.25*M_PI;
    maxDistance_ = 30;
    value_ = new float[nVal_];
}

rgba_vision::~rgba_vision()
{
    //dtor
    delete[] value_;
}

void rgba_vision::update(const bot &bot_, int &nVal, float *&Value)
{
    color cArray_[nPixel_];
    for(int i = 0; i < nPixel_; i++)
        cArray_[i] = {10,10,10};

    //make new vector with interesting objects:

    vector<object*> vVisible;
    vVisible.reserve(gWorld.size());

    copy_if(gWorld.begin(),gWorld.end(),back_inserter(vVisible), InFOV(&bot_,this));
    copy_if(gBots.begin(),gBots.end(),back_inserter(vVisible), InFOV(&bot_,this));

    sort(vVisible.begin(), vVisible.end(),
        [=](object *a, object *b){
            float diff_a = (a->Pos_ - bot_.Pos_).absqr();
            float diff_b = (b->Pos_ - bot_.Pos_).absqr();
            return diff_a>diff_b;}
            );

    for(auto it = vVisible.begin(); it != vVisible.end(); ++it)
    {
        Coord Diff = (*it)->Pos_ - bot_.Pos_;
        Coord DiffPerpUnit = (Diff.perp()).unit();
        Coord DiffA = Diff + (*it)->size_*DiffPerpUnit;
        Coord DiffB = Diff - (*it)->size_*DiffPerpUnit;

        Coord RightFOV = bot_.Direction_; RightFOV.rotate(-openingAngle_);

        float betaA = DiffA.Theta360(RightFOV);
        float betaB = DiffB.Theta360(RightFOV);

        int iPosA = betaA/(2*openingAngle_)*nPixel_;
        int iPosB = betaB/(2.*openingAngle_)*nPixel_;
//        cout<<iPosA<<"......."<<iPosB<<endl;
        if(iPosA>=nPixel_)
        {
            if(iPosB >= nPixel_)
            {
                if(iPosA>=iPosB) continue;
                iPosA = nPixel_-1;
                iPosB = 0;
            }
            iPosA = nPixel_-1;
        } else
        {
            if(iPosB >= nPixel_)
            {
                if(iPosA>=iPosB) continue;
                iPosB = 0;
            }
        }

        for(int i = iPosB; i < iPosA; i++)
        {
                cArray_[i] = (*it)->c_;
        }
    }

//    for(int i = 0; i < nPixel_; i++)
//    {
//        renderer::Instance()->Rect({i,0}, {1,1}, cArray_[i]);
//    }

    for(int i = 0; i<nPixel_; i++)
    {
        value_[i] = cArray_[i].b/255.;
        value_[i+nPixel_] = cArray_[i].r/255.;
        value_[i + 2*nPixel_] = cArray_[i].g/255.;
    }


    Value = value_;
    nVal = nVal_;
}


bool rgba_vision::InFOV::operator()(const object *obj) const
{
    if(!obj->active_) return false;
    if((bot*)obj == bot_) return false;

    Coord Diff = obj->Pos_ - bot_->Pos_;
    if( Diff.abs() > caller_->maxDistance_ ) return false;

    if(Diff.Theta180(bot_->Direction_) > caller_->openingAngle_+atan(obj->size_/Diff.abs())) return false;
    return true;
}
