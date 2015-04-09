#ifndef _BOT_H
#define _BOT_H

#include<vector>
#include"Coord.h"
#include"object.h"
#include"sensor.h"
#include"toolbox.h"
#include"doublefann.h"
#include"fann_cpp.h"

using namespace FANN;
using namespace std;

class bot: public object
{

    public:
    Coord Vel_;

    float Life_;
    float MaxLife_;

    unsigned Lifetime_;


    unsigned nSensorOut_;
    vector<sensor*> sensors_;
    neural_net *nnet_;



    void init();
    void update();
    void mutate();


    bot(Coord);
    bot(const bot&);
    bot( bot&& );
//
//    bot* operator=(const bot&);
//    bot* operator=(bot&&);

    ~bot();
    private:
    void copy_bot(const bot&); //copy non-dynamic members
    void copy_nn(const bot&);

};


#endif
