#include "observerManager.h"

observerManager::observerManager()
{
    //ctor
}


void observerManager::deleteAll()
{
    for(auto it = observerList_.begin(); it != observerList_.end(); ++it)
        delete *it;
    observerList_.clear();

}
