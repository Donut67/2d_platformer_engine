#include "../include/raylib.h"
#include "../include/Transform.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void TransformComp::findThis_i(const NTree<GameObject> &a, Vector2 &vec, bool &ended) const{
    if(a.content() != NULL) {
        Vector2 this_pos = (*a.content())[TransformComp()]->getPosition();
        vec = Vector2{vec.x + this_pos.x, vec.y + this_pos.y};
        findThis_i(a.content()->getParent(), vec, ended);
    }
}

Vector2 TransformComp::findThis() const{
    Vector2 vec = _position;
    bool found = false;
    
    findThis_i(_gameObject->getParent(), vec, found);

    return vec;
}

TransformComp::TransformComp(){

}

TransformComp::TransformComp(Vector2 pos, Vector2 scale){
    _position = pos;
    _scale = scale;
}

// DESTRUCTORS

// MODIFIERS
void TransformComp::update(){
    
}

void TransformComp::setPosition(Vector2 pos){
    _position = pos;
}

void TransformComp::setScale(Vector2 scale){
    _scale = scale;
}

void TransformComp::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

// CONSULTORS
Vector2 TransformComp::getPosition() const{
    return _position;
}

Vector2 TransformComp::getGlobalPosition() const{
    return findThis();
}

Vector2 TransformComp::getScale() const{
    return _scale;
}

// DRAW
void TransformComp::draw(){

}