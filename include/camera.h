#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class TrackCamera : public Component{
    public:
        // CONSTRUCTORS
        TrackCamera();

        // DESTRUCTORS

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void update();

        // CONSULTORS
        void draw();

    private:
        shared_ptr<GameObject> _gameObject;

};
#endif