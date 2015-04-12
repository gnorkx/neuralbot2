#ifndef SIMPLE_EYES_H
#define SIMPLE_EYES_H

#include "sensor.h"
#include "bot.h"
#include <algorithm>


extern vector<object*>gWorld;

class simple_eyes : public sensor
{
    public:

    void update(const bot&,int&,float*&);
        simple_eyes();
        ~simple_eyes();

    protected:
    private:

    float *value_;
    static simple_eyes *instance_;
};

#endif // SIMPLE_EYES_H
