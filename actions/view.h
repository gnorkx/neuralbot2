#ifndef VIEW_H
#define VIEW_H

#include <action.h>


class view : public action
{
    public:
        view();
        ~view();
        void perform(bot&, int, float*);
        unsigned GetNRepresentations() const {return nRepresentations_;};
    protected:
    private:
    int nRepresentations_; //number of actions represented by single class/object (eg Move class has 4 directions)
};


#endif // VIEW_H
