#ifndef RGBA_VISION_H
#define RGBA_VISION_H

#include "sensor.h"
#include <vector>
#include <algorithm>
#include "../tools/color.h"
#include "../objects/object.h"
#include "../objects/bot.h"


class rgba_vision : public sensor
{
    public:

        float *value_;
        rgba_vision();
        ~rgba_vision();
        void update(const bot&, int&, float*&);
    protected:
    private:
    float openingAngle_;
    float maxDistance_;
    int nPixel_;

    struct InFOV
    {
        InFOV(const bot* bot, const rgba_vision* caller ):bot_(bot), caller_(caller){};
        bool operator()(const object*) const;
        private:
            const bot* bot_;
            const rgba_vision* caller_;
    };
};

#endif // RGBA_VISION_H
