#include "../include/raylib.h"
#include "../include/sprite.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

Sprite::Sprite(){
    
}

Sprite::Sprite(shared_ptr<Texture2D> texture, float scale){
    _texture = texture;
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

void Sprite::draw(){
    Vector2 pos = (*_gameObject)[TransformComp()]->getGlobalPosition();
    DrawTextureEx(*_texture, pos, 0.0f, _scale, WHITE);
}