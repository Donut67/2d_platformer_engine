#include "raylib.h"
#include "animation.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

using namespace std;

float Animation::solveSin(float ini, float end, float time, float act){
    float a = abs(ini - end);
    if(ini > end) a = -a;
    float k = -a + end;
    return a * sin(0.5f / time * (act * 2 * PI - PI * time)) + k;
}

float Animation::solveRamp(float ini, float end, float time, float act){
    float a = ini;
    float m = abs(ini - end) / time;
    return m * act + a;
}

Animation::Animation(){

}

Animation::Animation(string meta){
    _started = _ended = false;
    _actual = 0;
    _intime = 0.0f;
    ifstream outfile("resources/" + meta);
    string line;
    while(outfile){
        getline(outfile, line);
        vector<string> aux = splice(line, ' ');
        _tracks.push_back(animationTrack{stof(aux[0]), aux[1], aux[2], aux[3], stof(aux[4]), stof(aux[5])});
    }
}

Animation::~Animation(){
    
}

void Animation::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void Animation::start(){
    _intime = 0;
    _started = true;
    _ended   = false;
}

void Animation::restart(){
    if(_tracks[_actual].component == "transform"){
        shared_ptr<TransformComp> trans = (*_gameObject)[TransformComp()];
        
        if(_tracks[_actual].part == "posx") {
            Vector2 pos = trans->getGlobalPosition();
            pos.x = _tracks[_actual].initial;
            trans->setPosition(pos);
        }else if(_tracks[_actual].part == "posy") {
            Vector2 pos = trans->getGlobalPosition();
            pos.y = _tracks[_actual].initial;
            trans->setPosition(pos);
        }else if(_tracks[_actual].part == "scax") {
            Vector2 sca = trans->getScale();
            sca.x = _tracks[_actual].initial;
            trans->setScale(sca);
        }else if(_tracks[_actual].part == "scay") {
            Vector2 sca = trans->getScale();
            sca.y = _tracks[_actual].initial;
            trans->setScale(sca);
        }
    }else if(_tracks[_actual].component == "paint"){
        shared_ptr<Paint> trans = (*_gameObject)[Paint()];

        Color c = trans->getColor();
        float aux = _tracks[_actual].initial;

        if(_tracks[_actual].part == "r") c.r = (unsigned char)int(aux);
        else if(_tracks[_actual].part == "g") c.g = (unsigned char)int(aux);
        else if(_tracks[_actual].part == "b") c.b = (unsigned char)int(aux);
        else if(_tracks[_actual].part == "a") c.a = (unsigned char)int(aux);
        
        trans->setColor(c);
    }
    _ended = _started = false;
    _intime = 0;
}

void Animation::stop(){
    _started = false;
    _ended   = true;
}

void Animation::update(){
    if(_started){
        if(_tracks[_actual].component == "transform"){
            shared_ptr<TransformComp> trans = (*_gameObject)[TransformComp()];
            
            float aux = 0.0f;
            if(_intime >= _tracks[_actual].time) aux = _tracks[_actual].ending;
            else if(_tracks[_actual].type == "sin") aux = solveSin(_tracks[_actual].initial, _tracks[_actual].ending, _tracks[_actual].time, _intime);
            else if(_tracks[_actual].type == "ramp") aux = solveRamp(_tracks[_actual].initial, _tracks[_actual].ending, _tracks[_actual].time, _intime);
            
            if(_tracks[_actual].part == "posx") {
                Vector2 pos = trans->getGlobalPosition();
                pos.x = aux;
                trans->setPosition(pos);
            }else if(_tracks[_actual].part == "posy") {
                Vector2 pos = trans->getGlobalPosition();
                pos.y = aux;
                trans->setPosition(pos);
            }else if(_tracks[_actual].part == "scax") {
                Vector2 sca = trans->getScale();
                sca.x = aux;
                trans->setScale(sca);
            }else if(_tracks[_actual].part == "scay") {
                Vector2 sca = trans->getScale();
                sca.y = aux;
                trans->setScale(sca);
            }
        }else if(_tracks[_actual].component == "paint"){
            shared_ptr<Paint> trans = (*_gameObject)[Paint()];

            Color c = trans->getColor();
            float aux = 0.0f;
            if(_intime >= _tracks[_actual].time) aux = _tracks[_actual].ending;
            else if(_tracks[_actual].type == "sin") aux = solveSin(_tracks[_actual].initial, _tracks[_actual].ending, _tracks[_actual].time, _intime);
            else if(_tracks[_actual].type == "ramp") aux = solveRamp(_tracks[_actual].initial, _tracks[_actual].ending, _tracks[_actual].time, _intime);

            if(_tracks[_actual].part == "r") c.r = (unsigned char)int(aux);
            else if(_tracks[_actual].part == "g") c.g = (unsigned char)int(aux);
            else if(_tracks[_actual].part == "b") c.b = (unsigned char)int(aux);
            else if(_tracks[_actual].part == "a") c.a = (unsigned char)int(aux);
            
            trans->setColor(c);
        }

        _intime += GetFrameTime();
        if(_intime >= _tracks[_actual].time) {
            _started = false;
            _ended   = true;
            _intime = 0;
        }
    }
}

bool Animation::isStarted() const{
    return _started;
}

bool Animation::isEnded() const{
    return _ended;
}

void Animation::draw(){

}