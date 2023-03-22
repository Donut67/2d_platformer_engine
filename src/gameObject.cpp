#include "raylib.h"
#include "gameObject.h"
#include <cstdlib>
#include <iostream>
#include <variant>
#include <vector>
#include <memory>

using namespace std;

// CONSTRUCTORS
GameObject::GameObject(){
    _name = "NULL";
    _shouldClose = false;
}

GameObject::GameObject(string name, shared_ptr<NTree<GameObject>> root){
    _name = name;
    _root = root;
    _enabled = true;
    _shouldClose = _isPaused = false;
}

// DESTRUCTORS

// MODIFIERS
void GameObject::closeWindow(){
    _shouldClose = true;
}

void GameObject::setEnable(const bool &enable){
    _enabled = enable;
}

void GameObject::setPaused(const bool &paused){
    _isPaused = paused;
}

void GameObject::addComponent(shared_ptr<Component> comp){
    _compList.push_back(comp);
    comp->setGameObject(shared_from_this());
}

void GameObject::update(){
    for(auto i : _compList) i->update();
}

// CONSULTORS
string GameObject::name() const{
    return _name;
}

bool GameObject::isEnable() const{
    return _enabled;
}

bool GameObject::isPaused() const{
    return _isPaused;
}

bool GameObject::WindowShouldClose() const{
    return _shouldClose;
}

vector<shared_ptr<Component>> GameObject::getComponents() const{
    return _compList;
}

shared_ptr<Component> GameObject::operator [](const int &pos) const{
    return _compList[pos];
}

shared_ptr<NTree<GameObject>> GameObject::getRoot() const{
    return _root;
}

NTree<GameObject> GameObject::getParent() const{
    NTree<GameObject> res;
    int i = 1;
    bool found = false;

    while(!found && i <= _root->nBrothers()){
        found = _root->brother(i).content()->name() == _name;
        if(!found) i++;
    }

    if(!found){
        i = 0;
        NTree<GameObject> aux;
        bool ended = false;
        while(!found && i <= _root->nBrothers()){
            aux = getParent_i(_root->brother(i), found, ended);
            if(!found) i++;
        }
        if(found) return aux;
    }

    return NTree<GameObject>();
}

NTree<GameObject> GameObject::find_this() const{
    NTree<GameObject> res;
    int i = 1;
    bool found = false;

    while(!found && i <= _root->nBrothers()){
        find_i(_root->brother(i), found, _name);
        i++;
    }
    
    return res;
}

NTree<GameObject> GameObject::find(const string &name) const{
    NTree<GameObject> res;
    int i = 1;
    bool found = false;

    while(!found && i <= _root->nBrothers()){
        res = find_i(_root->brother(i), found, name);
        i++;
    }
    
    return res;
}

void GameObject::draw() const{
    for(auto i : _compList) i->draw();
}

NTree<GameObject> GameObject::getParent_i(const NTree<GameObject> &a, bool &found, bool &ended) const{
    NTree<GameObject> aux;
    if(!a.isEmpty() && a.content()->name() != _name){
        int i = 1;
        while(!found && i <= a.nChilds()){
            aux = getParent_i(a.child(i), found, ended);
            i++;
        }

        if(ended){
            ended = false;
            return a;
        }
    } else if(!a.isEmpty() && a.content()->name() == _name) {
        found = true;
        ended = true;
    }
    return aux;
}

NTree<GameObject> GameObject::find_i(const NTree<GameObject> &a, bool &found, const string &name) const{
    NTree<GameObject> aux;
    if(!a.isEmpty() && a.content()->name() != name){
        int i = 1;
        while(!found && i <= a.nChilds()){
            aux = find_i(a.child(i), found, name);
            i++;
        }
    } else if(!a.isEmpty() && a.content()->name() == name){
        found = true;
        aux = a;
    }
    return aux;
}