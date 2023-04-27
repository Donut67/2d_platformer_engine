#include "raylib.h"
#include "inspector.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void Inspector::update() {
    Window::update();
}

void Inspector::setObservers(shared_ptr<GameObject> go) {
    _tracker.clearObservers();
    if(go != nullptr) {
        _tracker.setObserver(_observerList[0]);
        for(auto c : go->getComponents()){
            if(dynamic_pointer_cast<TransformComp>(c) != nullptr) _tracker.setObserver(_observerList[1]);
            if(dynamic_pointer_cast<AnimatedSprite>(c) != nullptr) _tracker.setObserver(_observerList[2]);
            if(dynamic_pointer_cast<RigidBody>(c) != nullptr) _tracker.setObserver(_observerList[3]);
            if(dynamic_pointer_cast<AABB>(c) != nullptr) _tracker.setObserver(_observerList[4]);
        }
        _tracker.setGameObject(go);
    }
}

void Inspector::draw() {
    Window::draw();

    _tracker.notify(Vector2{_position.x, _position.y + 30.0f});
    // DrawTextEx(_font, "The inspector is cleared", Vector2{_position.x + 5, _position.y + 35}, 28, 0, BLACK);
}