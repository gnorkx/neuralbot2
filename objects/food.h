#ifndef _FOOD_H
#define _FOOD_H

#include "object.h"
#include "../observer/subject.h"


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
#include "../observer/stats.h"
#endif // _FOOD_H
