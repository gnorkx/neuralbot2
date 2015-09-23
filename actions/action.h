#ifndef ACTION_H
#define ACTION_H


class bot;

class action
{
    public:
        virtual ~action() {};
        virtual void perform(bot&, int, float*) = 0;
        virtual unsigned GetNRepresentations() const = 0;
    protected:
    private:
    int nRepresentations_; //number of actions represented by single class/object (eg Move class has 4 directions)
};

#endif // ACTION_H
