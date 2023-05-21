#include "raylib.h"
#include "button.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

Button::Button(){

}

Button::Button(Color color){
    _color = _hovercolor = color;
    _hover = _pressed = false;
}

Button::Button(Color color, Color hover){
    _color =  color;
    _hover = _pressed = false;
    _hovercolor = hover;
}

void Button::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void Button::setColor(Color color){
    _color = color;
}

void Button::update(){
    
}

bool Button::isHover() const{
    Vector2 pos  = (*_gameObject)[TransformComp()]->getGlobalPosition();
    Vector2 size = (*_gameObject)[TransformComp()]->getScale();

    return CheckCollisionPointRec(GetMousePosition(), Rectangle{pos.x, pos.y, size.x, size.y});;
}

bool Button::isPressed() const{
    return isHover() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

Color Button::getColor() const{
    return _color;
}

Color Button::getHoverColor() const{
    return _hovercolor;
}

void Button::draw(){
    Vector2 pos  = (*_gameObject)[TransformComp()]->getGlobalPosition();
    Vector2 size = (*_gameObject)[TransformComp()]->getScale();
    DrawRectangle(pos.x, pos.y, size.x, size.y, _color);
}