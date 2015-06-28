#include "subject.h"

subject::subject()
{
    //ctor
    //add observers
    nObservers_ =observerManager::Instance()->nObserver();
    observers_ = new observer*[nObservers_];
    for(int i = 0; i< nObservers_; i++)
        observers_[i] = observerManager::Instance()->GetObserver(i);


}

subject::~subject()
{
    //dtor
    delete[] observers_;
}

void subject::notify(const object* obj, int e)
{
    for(int i = 0; i < nObservers_ ; i++)
        observers_[i]->onNotify(obj, e);
}
