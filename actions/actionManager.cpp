#include "actionManager.h"
#include<algorithm>
#include<iostream>
#include"../tools/toolbox.h"
#include "move.h"
#include "view.h"

actionManager::actionManager()
{
    //ctor
    actionList_.clear();
    actionList_.push_back(new move);
    //actionList_.push_back(new view);

    nMaxRepresentations_=0;
    for(auto it : actionList_)
        nMaxRepresentations_ += it->GetNRepresentations();
}

actionManager::~actionManager()
{
    //dtor
    for(auto it : actionList_)
        delete it;
    actionList_.clear();
}

void actionManager::ChoseAction(bot& b, int nnnet_out, double* nnet_out)
{

        int nIn = std::min(nnnet_out, nMaxRepresentations_);
        int idx = GetMax(nIn, nnet_out);
        for( auto it : actionList_ )
        {
            if(idx > it->GetNRepresentations())
                idx-=it->GetNRepresentations();
            else{
                it->perform(b, idx, nullptr);
                break;
            }
        }

}

int actionManager::GetMax(int idx, double* nnet_out)
{

    double fmax= -1e10;
    int imax = -1;
    for(int i = 0 ; i<idx; i++)
    {
        if(nnet_out[i]>fmax)
        {
            fmax = nnet_out[i];
            imax = i;
        }
    }
    return std::max(0,imax);


    //alternative (Probability)
/*
    double *fnew = new double[idx];
    for(int i = 0; i< idx; i++)
        fnew[i] = exp(nnet_out[i]);

    double sum = 0;
    for(int i = 0; i< idx; i++)
        sum+=fnew[i];
    for(int i = 0; i< idx; i++)
        fnew[i]/=sum;

    float rand = rnd();
    for(int i = 0; i< idx; i++)
    {
        if(rand>fnew[i]) rand-=fnew[i];
        else return i;
    }
    return idx;
    */
}
