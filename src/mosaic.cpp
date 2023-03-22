#include "../include/raylib.h"
#include "../include/mosaic.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

Mosaic::Mosaic(){

}

Mosaic::Mosaic(shared_ptr<Texture2D> texture, float scale){
    _texture = texture;
    _scale = scale;

    _tileSize = Vector2{_texture->width / 3.0f, _texture->height / 3.0f};
}

Mosaic::~Mosaic(){
    
}

void Mosaic::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void Mosaic::update(){
    
}

void Mosaic::draw(){
    Vector2 pos  = (*_gameObject)[TransformComp()]->getGlobalPosition();
    Vector2 size = (*_gameObject)[TransformComp()]->getScale();

    Vector2 inSize{size.x - (_tileSize.x * 2.0f * _scale), size.y - (_tileSize.y * 2.0f * _scale)};
    // cout<<inSize.x<<" "<<inSize.y<<"\n";

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            Vector2 new_pos, new_size{_tileSize.x * _scale, _tileSize.y * _scale};

            if(i == 0)      new_pos.x =                            0.0f + pos.x;
            else if(i == 1) new_pos.x =            _tileSize.x * _scale + pos.x;
            else if(i == 2) new_pos.x = _tileSize.x * _scale + inSize.x + pos.x;

            if(j == 0)      new_pos.y =                            0.0f + pos.y;
            else if(j == 1) new_pos.y =            _tileSize.y * _scale + pos.y;
            else if(j == 2) new_pos.y = _tileSize.y * _scale + inSize.y + pos.y;

            if(i == 1) new_size.x = inSize.x;
            if(j == 1) new_size.y = inSize.y;
            
            Rectangle src = Rectangle{i * _tileSize.x, j * _tileSize.y, _tileSize.x, _tileSize.y};
            Rectangle dst = Rectangle{new_pos.x, new_pos.y, new_size.x, new_size.y};
            DrawTexturePro(*_texture, src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
        }
    }
}