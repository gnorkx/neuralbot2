#ifndef _BOT_H
#define _BOT_H

#include<vector>
#include"../tools/Coord.h"
#include"object.h"
#include"../sensors/sensor.h"
#include"../tools/toolbox.h"
#include"doublefann.h"
#include"fann_cpp.h"

#include"food.h"
#include"../observer/subject.h"
#include"../observer/stats.h"
#include"../actions/actionManager.h"

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



    bot(Coord);
    bot(const bot&);
    bot( bot&& );
//
//    bot* operator=(const bot&);
//    bot* operator=(bot&&);

    ~bot();

    float GetFitness() const;

    subject subject_;

    void mutate();

    private:
        void copy_bot(const bot&); //copy non-dynamic members
        void copy_nn(const bot&);

        object* touch();
        void eat(food*);
        void learn();

        fann_type **nnet_TrainIn_;
        fann_type **nnet_TrainOut_;

        actionManager actionM_;


        const static unsigned nnet_TrainSteps_ = 100;
         unsigned nnet_nInputs_ = 2;
        const static unsigned nnet_nOutputs_ = 4;
        void clone();

        void kill();
        bot* newChild();

        static const float LIFE_REDUCTION;



};


#endif
