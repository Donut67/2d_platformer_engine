#ifndef __TRACKER_H__
#define __TRACKER_H__

#include "subject.h"
#include "observer.h"
// #include "component.h"
#include "gameObject.h"

#include <memory>
#include <set>

// class GameObject;

class Tracker : public Subject {
    private:
        set<shared_ptr<Observer>> _observers;
        shared_ptr<GameObject> _go;
    
    public:
        Tracker();
        Tracker(const shared_ptr<GameObject>& go);
        void setObserver(const shared_ptr<Observer>& o);
        void remObserver(const shared_ptr<Observer>& o);
        void setGameObject(const shared_ptr<GameObject>& go);
        void notify();
};
#endif