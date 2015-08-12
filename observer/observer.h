#ifndef OBSERVER_H
#define OBSERVER_H

#include "../objects/object.h"


class observer
{
    public:
        virtual ~observer(){};
        virtual void onNotify(const object*, int) = 0;

};

#endif // OBSERVER_H
