#ifndef __TAB_BUTTON_BEHAVIOR_H__
#define __TAB_BUTTON_BEHAVIOR_H__

#include "raylib.h"
#include "gameObject.h"
#include "monoBehavior.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class GameObject;
class MonoBehavior;

class TabButtonBehavior : public MonoBehavior{
    public:
        // CONSTRUCTORS
        TabButtonBehavior();

        // DESTRUCTORS

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void update();

        // CONSULTORS
        void draw();

    private:

};
#endif