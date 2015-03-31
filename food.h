#ifndef _FOOD_H
#define _FOOD_H

#include "object.h"


class food: public object
{
    public:
        float life_;


        void update();

        food(Coord,float);
        ~food();
    protected:
    private:
};

#endif // _FOOD_H
