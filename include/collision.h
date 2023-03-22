#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class Collision : public Component{
    public:
        // [[ CONSTRUCTORS ]]
        Collision(){
            _static = true;
            _transparent = _isActive = false;
        }
        // pre:  true
        // post: default constructor

        // [[ DESTRUCTORS ]]

        // [[ MODIFIERS ]]

        // [[ CONSULTORS ]]
        virtual bool collides(shared_ptr<Collision> ref) const{
            return false;
        }
        // pre:  'ref' exists
        // post: checks if two collisions are overlaping 
        virtual void debug(){
            
        }
        // pre:
        // post: debugs the current collision

    protected:
        bool _isActive, _static, _transparent;

};
#endif