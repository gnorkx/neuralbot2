#ifndef _SENSOR_H
#define _SENSOR_H

class bot;


class sensor
{
    public:
    virtual void update(const bot&, int&, float*&) =0;
    virtual ~sensor(){};

    int nVal_;


};


#endif
