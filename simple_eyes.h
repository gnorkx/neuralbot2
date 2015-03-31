#ifndef SIMPLE_EYES_H
#define SIMPLE_EYES_H

#include "sensor.h"
#include "bot.h"
#include <algorithm>


extern vector<object*>gWorld;

class simple_eyes : public sensor
{
    public:


    void update(bot&,int&,float*);

        simple_eyes();
        ~simple_eyes();
    protected:
    private:
    int nVal_;
    float *value_;
};

#endif // SIMPLE_EYES_H
