#include "raylib.h"
#include "camera.h"
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

TrackCamera::TrackCamera(){

}

void TrackCamera::setGameObject(shared_ptr<GameObject> go){
    _gameObject = go;
}

void TrackCamera::update(){
    Vector2 pos      = (*_gameObject->getParent().content())[TransformComp()]->getGlobalPosition();
    Vector2 this_pos = (*_gameObject)[TransformComp()]->getPosition();
    Vector2 root_pos = (*_gameObject->getRoot()->content())[TransformComp()]->getPosition();
    (*_gameObject->getRoot()->content())[TransformComp()]->setPosition(Vector2{root_pos.x - pos.x + this_pos.x, root_pos.y - pos.y + this_pos.y});
}

void TrackCamera::draw(){

}