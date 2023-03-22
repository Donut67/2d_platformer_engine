#include "raylib.h"
#include "rigidbody.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

RigidBody::RigidBody(){
    force = Vector2{0.0f, 0.0f};
    mass = 10;
    linearVelocity = Vector2{0.0f, 0.0f};
}

void RigidBody::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void RigidBody::addForce(Vector2 f){
    force = Vector2{force.x + f.x, force.y + f.y};
}

void RigidBody::setVelocity(Vector2 v){
    linearVelocity = v;
}

void RigidBody::update(){
    linearAcceleration = Vector2{force.x / mass, force.y / mass};

    // Vector2 dir{0.0f, 0.0f};
    // Vector2 dir = (*_gameObject)[AABB()]->getDirection();
    // if(dir.x != 0) linearVelocity.x = 0;
    // else linearVelocity.x += linearAcceleration.x * GetFrameTime();
    linearVelocity.x += linearAcceleration.x * GetFrameTime();
    // if(dir.y != 0) linearVelocity.y = 0;
    // else linearVelocity.y += linearAcceleration.y * GetFrameTime();
    linearVelocity.y += linearAcceleration.y * GetFrameTime();

    Vector2 posinit = (*_gameObject)[TransformComp()]->getPosition();
    (*_gameObject)[TransformComp()]->setPosition(Vector2{posinit.x + linearVelocity.x * GetFrameTime(), posinit.y + linearVelocity.y * GetFrameTime()});

    force = Vector2{0.0f, 800.0f};
}

Vector2 RigidBody::getVelocity() const{
    return linearVelocity;
}

void RigidBody::draw()
{
}