#include "stats.h"

stats::stats()
{
    //ctor
    nBots_ = 0;
    vAgeDie.clear();

    nFood_ = 0;

    outfile_.open("stats_out.txt", std::ofstream::out);
    iWrites_ = 0;

}

stats::~stats()
{
    //dtor
    outfile_.close();
}

void stats::onNotify(const object *obj, int evnt)
{

    if(evnt == event::eat)
    {}
    else if(evnt == event::born)
    {
        nBots_++;
    }
    else if(evnt == event::died)
    {
        bot* b = (bot*)obj;
        nBots_--;
        if(vAgeDie.size()>10)
            vAgeDie.pop_front();
        vAgeDie.push_back(b->Lifetime_);

        outfile_<<iWrites_<<"\t"<<nBots_<<"\t"<<nFood_<<"\t"<<average(vAgeDie)<<"\n";
        iWrites_++;
    }
    else if(evnt == event::inactive)
    {
        nFood_--;
    }
    else if(evnt == event::active)
    {
        nFood_++;
    }

}


float stats::average(std::deque<float> &v)
{
    float sum = 0;
    for(int i = 0; i< v.size();++i)
        sum += v[i];
    return sum/v.size();
}
