#include "../include/tracker.h"

Tracker::Tracker() {
    _go = nullptr;
}

Tracker::Tracker(const shared_ptr<GameObject>& go) {
    _go = go;
}

void Tracker::setObserver(const shared_ptr<Observer>& o){
    _observers.insert(o);
}

void Tracker::remObserver(const shared_ptr<Observer>& o){
    _observers.erase(o);
}

void Tracker::setGameObject(const shared_ptr<GameObject>& go) {
    _go = go;
}

void Tracker::notify(){
    Data data;
    if(_go != nullptr) {
        data.name = _go->name();
        // data.position = _comp.getPosition();
        // data.scale = _comp.getScale();

        for(auto o : _observers) o->update(data);
    }
}
