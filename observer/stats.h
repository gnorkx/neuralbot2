#ifndef STATS_H
#define STATS_H

#include<deque>
#include<fstream>


#include "observer.h"
#include "bot.h"


class stats : public observer
{
    public:
        stats();
        ~stats();
        void onNotify(const object*, int);

        static enum event {born, died, eat, active, inactive} event_;

    private:
        unsigned nBots_;
        std::deque<float> vAgeDie;

        float average(std::deque<float>&);

        unsigned nFood_;


        std::ofstream outfile_;
        unsigned iWrites_;

};


#endif // STATS_H
