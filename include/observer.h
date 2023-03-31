#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "data.h" 

class Observer { 
    public: 
        virtual void update(const Data &data) = 0; 
};
#endif