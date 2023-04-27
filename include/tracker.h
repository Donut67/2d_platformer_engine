#ifndef __TRACKER_H__
#define __TRACKER_H__

#include "subject.h"
#include "observer.h"
// #include "component.h"
#include "gameObject.h"
#include "viewGameobject.h"
#include "viewTransform.h"
#include "viewTransform.h"
#include "viewAnimatedsprite.h"
#include "viewRigidbody.h"
#include "viewAABB.h"

#include <memory>
#include <vector>

// class GameObject;

class Tracker : public Subject {
    private:
        vector<shared_ptr<Observer>> _observers;
        shared_ptr<GameObject> _go;
    
    public:
        Tracker();
        Tracker(const shared_ptr<GameObject>& go);
        void setObserver(const shared_ptr<Observer>& o);
        void remObserver(const shared_ptr<Observer>& o);
        void clearObservers();
        void setGameObject(const shared_ptr<GameObject>& go);
        void notify(const Vector2 &pos);
};
#endif