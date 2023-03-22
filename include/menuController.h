#ifndef __MENU_CONTROLLER_H__
#define __MENU_CONTROLLER_H__

#include "raylib.h"
#include "gameObject.h"
#include "monoBehavior.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class MonoBehavior;

class MenuController : public MonoBehavior{
    public:
        // CONSTRUCTORS
        MenuController();

        // DESTRUCTORS

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void update();

        // CONSULTORS
        
        void draw();

    private:
        bool _isActive;
        string _actual;

        shared_ptr<GameObject> _gameObject;

};
#endif