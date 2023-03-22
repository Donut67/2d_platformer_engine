#include "../include/raylib.h"
#include "../include/tabButtonBehavior.h"
#include <cstdlib>
#include <iostream>

using namespace std;

TabButtonBehavior::TabButtonBehavior(){

}

void TabButtonBehavior::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void TabButtonBehavior::update(){
    auto anim   = (*_gameObject)[Animation()];
    auto button = (*_gameObject)[Button()];
    if(button->isHover() && !anim->isStarted() && !anim->isEnded()) anim->start();
    else if(!button->isHover() && anim->isEnded()) anim->restart();
}

void TabButtonBehavior::draw(){

}