#include "../include/raylib.h"
#include "../include/settingsButtonBehavior.h"
#include <cstdlib>
#include <iostream>

using namespace std;

SettingsButtonBehavior::SettingsButtonBehavior(){

}

void SettingsButtonBehavior::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void SettingsButtonBehavior::update(){
    if((*_gameObject)[Button()]->isPressed()) ToggleFullscreen();
}

void SettingsButtonBehavior::draw(){

}