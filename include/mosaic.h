#ifndef __MOSAIC_H__
#define __MOSAIC_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <utility>
#include <memory>

using namespace std;

class GameObject;
class Component;

class Mosaic : public Component{
    public:
        // CONSTRUCTORS
        Mosaic();
        Mosaic(shared_ptr<Texture2D> texture, float scale);

        // DESTRUCTORS
        ~Mosaic();

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);
        
        void update();

        // CONSULTORS
        void draw();

    private:
        shared_ptr<Texture2D> _texture;
        Vector2 _tileSize;
        string _filename;
        float _scale;

        shared_ptr<GameObject> _gameObject;
};
#endif