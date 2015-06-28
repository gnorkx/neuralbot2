#ifndef SUBJECT_H
#define SUBJECT_H

#include"observer.h"
#include"observerManager.h"

class subject
{
    public:
        subject();
        virtual ~subject();

        void notify(const object*, int);
        void addObserver();
        void removeAllObservers();

    protected:
    private:
        observer** observers_;
        int nObservers_;
};

#endif // SUBJECT_H
