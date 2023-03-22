#include "../include/raylib.h"
#include "../include/settingsContent.h"
#include <cstdlib>
#include <iostream>

using namespace std;

SettingsContent::SettingsContent(){
    _actual = "screen";
}

void SettingsContent::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void SettingsContent::update(){
    NTree<GameObject> tree1 = _gameObject->find("settingscontent");

    tree1.find("screensettingscontent").content()->setEnable(_actual == "screen");
    tree1.find("controlssettingscontent").content()->setEnable(_actual == "controls");

    if((*tree1.find("screensettingsbutton").content())[Button()]->isPressed()) _actual = "screen";
    if((*tree1.find("controlssettingsbutton").content())[Button()]->isPressed()) _actual = "controls";
}

void SettingsContent::draw(){

}