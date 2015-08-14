#include "status_feedback.h"
#include "../objects/bot.h"

status_feedback::status_feedback()
{
    //ctor
    nVal_ = 3;
    values_ = new float[nVal_];
}

status_feedback::~status_feedback()
{
    //dtor
    delete[] values_;

}

sensor* status_feedback::clone() const
{
    return new status_feedback();
}


void status_feedback::update(const bot &b, int &nVal, float* &values)
{


    values_[0] = b.Life_;
    values_[1] = b.Vel_.x;
    values_[2] = b.Vel_.y;

    values = values_;
    nVal = nVal_;

}
