#include "../include/tracker.h"

Tracker::Tracker() {
    _go = nullptr;
}

Tracker::Tracker(const shared_ptr<GameObject>& go) {
    _go = go;
}

void Tracker::setObserver(const shared_ptr<Observer>& o){
    _observers.push_back(o);
}

void Tracker::remObserver(const shared_ptr<Observer>& o){
    // _observers.erase(o);
}

void Tracker::clearObservers() {
    _observers.clear();
}

void Tracker::setGameObject(const shared_ptr<GameObject>& go) {
    _go = go;
}

void Tracker::notify(const Vector2 &pos){
    if(_go != nullptr) {
        Vector2 p = pos;
        for(auto o : _observers) {
            o->update((*_go), p);

            if(dynamic_pointer_cast<ViewGameobject>(o) != nullptr) p.y += 25;
            if(dynamic_pointer_cast<ViewTransform>(o) != nullptr) p.y += 75;
            if(dynamic_pointer_cast<ViewAnimatedSprite>(o) != nullptr) p.y += 100;
            if(dynamic_pointer_cast<ViewRigidBody>(o) != nullptr) p.y += 60;
            if(dynamic_pointer_cast<ViewAABB>(o) != nullptr) p.y += 100;
        }
    }
}