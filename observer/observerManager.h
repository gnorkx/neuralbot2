#ifndef OBSERVERMANAGER_H
#define OBSERVERMANAGER_H

#include<vector>
#include"observer.h"

class observerManager
{
    public:
        static observerManager* Instance()
            {static observerManager instance_; return &instance_;};
        void Add(observer* o)
            {observerList_.push_back(o);};
        void deleteAll();

        int nObserver()
            {return observerList_.size();};
        observer* GetObserver(unsigned i)
            {return i < observerList_.size() ? observerList_[i]:nullptr;};
    protected:
    private:
        observerManager();
        static observerManager instance_;
        vector<observer*> observerList_;
};

#endif // OBSERVERMANAGER_H
