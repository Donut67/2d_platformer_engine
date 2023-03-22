#ifndef __SELECTOR_H__
#define __SELECTOR_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class Selector : public Component{
    public:
        // CONSTRUCTORS
        Selector();

        // DESTRUCTORS
        ~Selector();

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void update();

        // CONSULTORS
        void draw();

    private:
        int _actual;
        bool _show;

        shared_ptr<GameObject> _gameObject;

};
#endif