#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "raylib.h"
#include "nTree.h"
#include "scene.h"
#include "gameObject.h"
#include "myFunctions.h"
#include <vector>
#include <string>

class GameObject;
class Scene;

class Component{
    public:
        Component(){}
        virtual ~Component(){}

        virtual void update(){cout<<"default update\n";}
        virtual void draw(){cout<<"default draw\n";}
        virtual void setGameObject(shared_ptr<GameObject> go){_gameObject = go;}

    protected:
        shared_ptr<GameObject> _gameObject;
};
#endif