#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H


#include<vector>
#include"action.h"

class bot;

class actionManager
{
    public:
        actionManager();
        virtual ~actionManager();
        void ChoseAction(bot&, int, double*);
    protected:
    private:
        std::vector<action*> actionList_;
        int nMaxRepresentations_;

        int GetMax(int, double*);
};

#endif // ACTIONMANAGER_H
