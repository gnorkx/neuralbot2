#ifndef _FOOD_H
#define _FOOD_H

#include "object.h"
#include "subject.h"


class food: public object
{
    public:
        float life_;

        subject subject_;



        void update();
        void SetInactive();

        food(Coord,float);
        ~food();
    protected:
    private:
    int inactiveCounter_;
};
#include "stats.h"
#endif // _FOOD_H
