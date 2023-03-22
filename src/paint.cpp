#include "raylib.h"
#include "paint.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

Paint::Paint(){

}

Paint::Paint(Color color){
    _color = color;
}

void Paint::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void Paint::setColor(const Color &color){
    _color = color;
}

void Paint::update(){
    
}
Color Paint::getColor() const{
    return _color;
}

void Paint::draw(){
    Vector2 pos  = (*_gameObject)[TransformComp()]->getGlobalPosition();
    Vector2 size = (*_gameObject)[TransformComp()]->getScale();
    DrawRectangle(pos.x, pos.y, size.x, size.y, _color);
}