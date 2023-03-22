#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class Sprite : public Component{
    public:
        // CONSTRUCTORS
        Sprite();
        Sprite(shared_ptr<Texture2D> texture, float scale);

        // DESTRUCTOR
        ~Sprite();
        
        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);

        void update();

        // CONSULTORS

        // DRAW
        void draw();

    private:
        shared_ptr<Texture2D> _texture;
        float _scale;

        shared_ptr<GameObject> _gameObject;

};
#endif