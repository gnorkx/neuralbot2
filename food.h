#ifndef _FOOD_H
#define _FOOD_H

#include "object.h"


class food: public object
{
    public:
        float life_;
        int active_;




        void update();
        void SetInactive();

        food(Coord,float);
        ~food();
    protected:
    private:
    int inactiveCounter_;
};

#endif // _FOOD_H
