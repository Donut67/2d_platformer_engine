#include "../include/raylib.h"
#include "../include/tileMap.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

TileMap::TileMap(){

}

TileMap::TileMap(TileSet set, Vector2 size, float scale, string mapMeta){
    _set     = set;
    _size    = size;
    _scale   = scale;
    _mapMeta = mapMeta;

    ifstream map("resources/" + mapMeta);
    int x, y;
    map>>x>>y;

    for(int i = 0; i < x; i++) for(int j = 0; j < y; j++) map>>_tileMap[pair<int, int>(j, i)];

    _tileSize = Vector2{set.getTexture()->width / _size.x, set.getTexture()->height / _size.y};
}

TileMap::~TileMap(){
    //UnloadTexture(_texture);
}

void TileMap::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void TileMap::update(){
    
}

string TileMap::getFileName() const{
    return _filename;
}

string TileMap::getMapMeta() const{
    return _mapMeta;
}

float TileMap::getScale() const{
    return _scale;
}

Vector2 TileMap::getSize() const{
    return _size;
}

Vector2 TileMap::getTileSize() const{
    return _tileSize;
}

void TileMap::draw(){
    Vector2 pos = (*_gameObject)[TransformComp()]->getGlobalPosition();
    for(auto i : _tileMap){
        if(i.second != -1){
            Vector2 new_pos = Vector2{pos.x + _tileSize.x * i.first.first * _scale, pos.y + _tileSize.y * i.first.second * _scale};
            Vector2 aux = _set[i.second];
            Rectangle src = Rectangle{aux.x * _tileSize.x, aux.y * _tileSize.y, _tileSize.x, _tileSize.y};
            Rectangle dst = Rectangle{new_pos.x, new_pos.y, _tileSize.x * _scale, _tileSize.y * _scale};
            DrawTexturePro(*_set.getTexture(), src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            // DrawText(to_string(i.second).c_str(), dst.x + dst.width/2, dst.y, 48, WHITE);
        }
    }
}