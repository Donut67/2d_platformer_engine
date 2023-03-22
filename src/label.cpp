#include "raylib.h"
#include "label.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

Label::Label(){

}

Label::Label(string text, shared_ptr<Font> font, char allign, int fontSize, Color color){
    _text = text;
    _allign = allign;
    _fontSize = fontSize;
    _color = color;
    _font = font;
}

Label::~Label(){
    //UnloadFont(_font);
}

void Label::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void Label::setText(string text){
    _text = text;
}

void Label::setColor(Color color){
    _color = color;
}

void Label::update(){

}

string Label::getText() const{
    return _text;
}

char Label::getAllign() const{
    return _allign;
}

int Label::getfontSize() const{
    return _fontSize;
}

Color Label::getColor() const{
    return _color;
}

void Label::draw(){
    Vector2 pos  = (*_gameObject)[TransformComp()]->getGlobalPosition();
    Vector2 size = (*_gameObject)[TransformComp()]->getScale();
    Vector2 this_pos = pos;
    
    if(_allign == 'c'){
        float length = _text.size() * (_fontSize / 9 * 5);
        this_pos = Vector2{pos.x + (size.x / 2) - (length / 2), pos.y};
    }else if(_allign == 'r'){
        float length = _text.size() * (_fontSize / 9 * 5);
        this_pos = Vector2{pos.x + size.x - length, pos.y};
    }
    
    DrawTextEx(*_font, _text.c_str(), Vector2{this_pos.x, this_pos.y}, _fontSize, 0, _color);
}