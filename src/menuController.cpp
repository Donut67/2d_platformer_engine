#include "raylib.h"
#include "menuController.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

MenuController::MenuController(){
    _isActive = false;
    _actual   = "inv";
}

void MenuController::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void MenuController::update(){
    if(IsKeyPressed(KEY_ESCAPE)) _isActive = !_isActive;

    NTree<GameObject> tree1 = _gameObject->find("escapemenu1");
    NTree<GameObject> tree2 = _gameObject->find("scenario1");

    auto anim = (*tree1.content())[Animation()];
    if(IsKeyPressed(KEY_ESCAPE) && _isActive) anim->start();

    tree1.content()->setEnable(_isActive); 
    tree2.content()->setPaused(_isActive);

    NTree<GameObject> invTab1   = tree1.find("inventorytab");
    NTree<GameObject> skillTab1 = tree1.find("skillstab");
    NTree<GameObject> mapTab1   = tree1.find("maptab");
    NTree<GameObject> settTab1  = tree1.find("settingstab");

    invTab1.find("inventorycontent").content()->setEnable(_actual == "inv");
    skillTab1.find("skillscontent").content()->setEnable(_actual == "skill");
    mapTab1.find("mapcontent").content()->setEnable(_actual == "map");
    settTab1.find("settingscontent").content()->setEnable(_actual == "set");

    if((*invTab1.find("inventorybutton").content())[Button()]->isPressed()) _actual = "inv";
    if((*skillTab1.find("skillsbutton").content())[Button()]->isPressed())  _actual = "skill";
    if((*mapTab1.find("mapbutton").content())[Button()]->isPressed())       _actual = "map";
    if((*settTab1.find("settingsbutton").content())[Button()]->isPressed()) _actual = "set";

    NTree<GameObject> quitButton1 = tree1.find("quitButton1");

    auto quit  = (*quitButton1.content())[Button()];
    auto label = (*quitButton1.content())[Label()];

    if(quit->isHover()) label->setColor(Color{122, 122, 122, 255});
    else label->setColor(Color{255, 255, 255, 255});

    if(quit->isPressed()) _gameObject->closeWindow();
}

void MenuController::draw(){

}