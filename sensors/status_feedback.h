#ifndef STATUS_FEEDBACK_H
#define STATUS_FEEDBACK_H

#include "sensor.h"


class status_feedback : public sensor
{
    public:
        status_feedback();
        ~status_feedback();
        void update(const bot&, int&, float*&);
        virtual sensor* clone() const;
    protected:
    private:

    float *values_;
};

#endif // STATUS_FEEDBACK_H
