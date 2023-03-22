#ifndef __RIGID_BODY_H__
#define __RIGID_BODY_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class RigidBody : public Component{
    public:
        // CONSTRUCTORS
        RigidBody();

        // DESTRUCTORS

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void addForce(Vector2 f);
        void setVelocity(Vector2 v);

        void update();

        // CONSULTORS

        Vector2 getVelocity() const;
        
        void draw();

    private:
        shared_ptr<GameObject> _gameObject;
        float mass;

        Vector2 linearVelocity, linearAcceleration, force;
};
#endif