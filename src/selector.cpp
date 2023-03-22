#include "raylib.h"
#include "selector.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

Selector::Selector(){
    _actual = 0;
    _show = false;
}

Selector::~Selector(){
    
}

void Selector::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void Selector::update(){
    auto sel = _gameObject->find("resolutionselector");

    if(_show && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        int i = 1;
        bool found = false;

        while(!found && i <= sel.nChilds()){
            found = (*sel.child(i).content())[Button()]->isPressed();
            if(!found) i++;
        }

        if(found) (*_gameObject)[Label()]->setText((*sel.child(i).content())[Label()]->getText());
        _show = false;
    }

    if((*_gameObject)[Button()]->isPressed()) _show = !_show;
    for(int i = 1; i <= sel.nChilds(); i++) sel.child(i).content()->setEnable(_show);
}

void Selector::draw(){
    
}