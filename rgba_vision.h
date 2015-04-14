#ifndef RGBA_VISION_H
#define RGBA_VISION_H

#include "sensor.h"
#include <vector>
#include <algorithm>
#include "color.h"
#include "object.h"
#include "bot.h"


class rgba_vision : public sensor
{
    public:

        float *value_;
        rgba_vision();
        ~rgba_vision();
        void update(const bot&, int&, float*&);
    protected:
    private:
};

#endif // RGBA_VISION_H
