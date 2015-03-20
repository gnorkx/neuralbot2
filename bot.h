#ifndef _BOT_H
#define _BOT_H

#include"object.h"
#include"sensor.h"
#include"doublefann.h"
#include"fann_cpp.h"

using namespace FANN;
using namespace std;

class bot: public object
{

    Coord Vel_;
    sensor sensors_[10];

    neural_net net_;



    void init();
    void update();

    bot();
    bot(const bot&);
    bot( bot&& );

    bot* operator=(const bot&);
    bot* operator=(bot&&);

    ~bot();

};


#endif
