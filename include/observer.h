#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "gameObject.h" 

class Observer { 
    public: 
        virtual void update(const GameObject &go, const float &height) = 0; 
};
#endif