#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include "observer.h"
#include "memory"
using namespace std;

class Subject {
    public:
        virtual void setObserver(const shared_ptr<Observer>& o) = 0;
        virtual void remObserver(const shared_ptr<Observer>& o) = 0;
        virtual void notify() = 0;
};
#endif