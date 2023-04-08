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

void Tracker::notify(){
    if(_go != nullptr) {
        float top = 0;
        for(auto o : _observers) {
            top += 10;
            o->update((*_go), top);

            if(dynamic_pointer_cast<ViewGameobject>(o) != nullptr) top += 50;
            if(dynamic_pointer_cast<ViewTransform>(o) != nullptr) top += 110;
            if(dynamic_pointer_cast<ViewAnimatedSprite>(o) != nullptr) top += 140;
            if(dynamic_pointer_cast<ViewRigidBody>(o) != nullptr) top += 80;
            if(dynamic_pointer_cast<ViewAABB>(o) != nullptr) top += 140;
        }
    }
}
