#include "raylib.h"
#include "rigidBody.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

RigidBody::RigidBody() {
    _gravity = 3720.0f;
    _mass = 100000.0f;
    _drag = 0.95f;
    _initialTime = 0.0f;
    _speed = Vector2{0.0f, 0.0f};
    _initialSpeed = Vector2{0.0f, 0.0f};
    _addedForce = Vector2{0.0f, 0.0f};
}

void RigidBody::setGameObject(shared_ptr<GameObject> go) {
    _gameObject = go;
}

void RigidBody::update() {
    Vector2 position = (*_gameObject)[TransformComp()]->getPosition();

    ComputeForce();
    Vector2 linearAcceleration = Vector2{_force.x / _mass, _force.y / _mass};
    _linearVelocity.x += linearAcceleration.x * GetFrameTime();
    _linearVelocity.y += linearAcceleration.y * GetFrameTime();

    position.x += _linearVelocity.x * GetFrameTime(); 
    position.y += _linearVelocity.y * GetFrameTime();

    (*_gameObject)[TransformComp()]->setPosition(position);

    Vector2 dir;
    if((*_gameObject)[AABB()]->isColliding(dir)){
        // Vector2 normal{-_force.x, -_force.y};
        Vector2 position = (*_gameObject)[TransformComp()]->getPosition();

        // linearAcceleration = Vector2{-_force.x / _mass, _force.y / _mass};
        // _linearVelocity.x -= linearAcceleration.x * GetFrameTime();
        // _linearVelocity.y -= linearAcceleration.y * GetFrameTime();
        if(dir.x != 0.0f) _linearVelocity.x = 0.0f;
        if(dir.y != 0.0f) _linearVelocity.y = 0.0f;

        position.x += dir.x; 
        position.y += dir.y;

        (*_gameObject)[TransformComp()]->setPosition(position);
    }
}

void RigidBody::addForce(Vector2 force) {
    _addedForce = Vector2{force.x + _addedForce.x, force.y + _addedForce.y};
}

void RigidBody::ComputeForce() {
    _addedForce.y += _mass * _gravity;
    _force = Vector2{0.0f, _mass * _gravity};
}

void RigidBody::setVelocity(Vector2 vel) {
    _linearVelocity = vel;
}

Vector2 RigidBody::getVelocity() const{
    return _linearVelocity;
}

void RigidBody::draw() {
    DrawText((to_string(_force.x) + "x, " + to_string(_force.y)).c_str(), 10, 40, 36, GREEN);
    DrawText((to_string(_linearVelocity.x) + "x, " + to_string(_linearVelocity.y)).c_str(), 10, 70, 36, GREEN);
}