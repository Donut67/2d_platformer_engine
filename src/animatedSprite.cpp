#include "../include/raylib.h"
#include "../include/animatedSprite.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

AnimatedSprite::AnimatedSprite(){

}

AnimatedSprite::AnimatedSprite(string filename, shared_ptr<Texture2D> texture, float scale, string mapMeta, float timeForFrame){
    _texture = texture;
    _scale = scale;
    _filename = filename;

    string line;
    ifstream file("resources/" + mapMeta);
    int anim = 0;
    getline(file, line);
    _size = Vector2{stof(line.substr(0, line.find(','))), stof(line.substr(line.find(',') + 1, line.size()))};

    getline(file, line);
    while(line != "-1"){
        bool found = false;
        int pos = 0;
        string value = "";
        vector<int> aux;

        while(!found && pos < int(line.size())){
            found = line[pos] == ',';
            if(line[pos] == ' '){
                aux.push_back(stoi(value));
                value = "";
            } else value += line[pos];
            if(found) aux.push_back(stoi(value));
            pos ++;
        }
        _animationMap[anim] = pair<vector<int>, bool>(aux, line.substr(pos) == " true");
        anim ++;
        getline(file, line);
    }

    _tileSize = Vector2{_texture->width / _size.x, _texture->height / _size.y};
    _timeForFrame = timeForFrame;
    _frameTime = _timeForFrame;
    _actual = _animation = 0;
    _fliph = _flipv = false;
}

AnimatedSprite::~AnimatedSprite(){
    //UnloadTexture(_texture);
}

void AnimatedSprite::setAnimation(int animation){
    _animation = animation;
}

void AnimatedSprite::setFlipH(bool flip){
    _fliph = flip;
}

void AnimatedSprite::setFlipV(bool flip){
    _flipv = flip;
}

void AnimatedSprite::update(){
    map<int, pair<vector<int>, bool>>::iterator it = _animationMap.find(_animation);
    if(it != _animationMap.end()) _cicle = it->second.first;
    else _cicle = _cicle;
    bool loop = it->second.second;

    _frameTime -= GetFrameTime();
    if(_frameTime <= 0){
        _frameTime = _timeForFrame;
        _actual ++;
    }
    _ended = _actual > (int)_cicle.size() - 1;

    if(loop && _ended) _actual = 0;
    else if(!loop && _ended) _actual = (int)_cicle.size() - 1; 
}

void AnimatedSprite::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

bool AnimatedSprite::ended() const{
    return _ended;
}

void AnimatedSprite::draw(){
    if(_texture != nullptr) {
        Vector2 pos = (*_gameObject)[TransformComp()]->getGlobalPosition();
        int i = _cicle[_actual] % (int)(_size.x), j = floor(_cicle[_actual] / (int)(_size.x));
        Rectangle src = Rectangle{
            i * _tileSize.x, 
            j * _tileSize.y, 
            (_fliph? -_tileSize.x : _tileSize.x), 
            (_flipv? -_tileSize.y : _tileSize.y)};
        Rectangle dst = Rectangle{pos.x, pos.y, _tileSize.x * _scale, _tileSize.y * _scale};
        DrawTexturePro(*_texture, src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    }
}