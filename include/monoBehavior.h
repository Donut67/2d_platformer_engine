#ifndef __MONO_BEHAVIOR_H__
#define __MONO_BEHAVIOR_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class MonoBehavior : public Component{
    public:
        // CONSTRUCTORS
        MonoBehavior(){}

        // DESTRUCTORS

        // MODIFIERS
        void update(){}

        // CONSULTORS

        void draw(){}

    private:
        
};
#endif