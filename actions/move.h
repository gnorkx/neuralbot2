#ifndef MOVE_H
#define MOVE_H

#include <action.h>


class move : public action
{
    public:
        move();
        virtual ~move();
        void perform(bot&, int, float*);
        unsigned GetNRepresentations() const {return nRepresentations_;};
    protected:
    private:
    int nRepresentations_; //number of actions represented by single class/object (eg Move class has 4 directions)
};

#endif // MOVE_H
