#include "../include/raylib.h"
#include "../include/sprite.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

Sprite::Sprite(){
    
}

Sprite::Sprite(string filename, shared_ptr<Texture2D> texture, float scale){
    _texture = texture;
    _filename = filename;
    _scale = scale;
}

Sprite::~Sprite(){
    //UnloadTexture(_texture);
}

void Sprite::update(){

}

void Sprite::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

float Sprite::scale() const {
    return _scale;
}

string Sprite::filename() const {
    return _filename;
}

shared_ptr<Texture2D> Sprite::texture() const {
    return _texture;
}

void Sprite::draw(){
    if(_texture != nullptr){
        Vector2 pos = (*_gameObject)[TransformComp()]->getGlobalPosition();
        DrawTextureEx(*_texture, pos, 0.0f, _scale, WHITE);
    }
}