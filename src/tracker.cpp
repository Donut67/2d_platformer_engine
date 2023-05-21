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

void Tracker::notify(Vector2 &pos){
    if(_go != nullptr) {
        for(auto o : _observers) {
            o->update(_go, pos);

            if(dynamic_pointer_cast<ViewGameobject>(o) != nullptr) pos.y += 27;
            else if(dynamic_pointer_cast<ViewTransform>(o) != nullptr) pos.y += 77;
            else if(dynamic_pointer_cast<ViewAnimatedSprite>(o) != nullptr) pos.y += 227;
            else if(dynamic_pointer_cast<ViewSprite>(o) != nullptr) pos.y += 202;
            else if(dynamic_pointer_cast<ViewRigidBody>(o) != nullptr) pos.y += 52;
            else if(dynamic_pointer_cast<ViewAABB>(o) != nullptr) pos.y += 102;
            else if(dynamic_pointer_cast<ViewButton>(o) != nullptr) pos.y += 127;
            else if(dynamic_pointer_cast<ViewLabel>(o) != nullptr) pos.y += 127;
            else if(dynamic_pointer_cast<ViewPaint>(o) != nullptr) pos.y += 52;
        }
    }
}