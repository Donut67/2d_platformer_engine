#ifndef __PAINT_H__
#define __PAINT_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class Paint : public Component{
    public:
        // CONSTRUCTORS
        Paint();
        Paint(Color color);

        // DESTRUCTORS

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);
        void setColor(const Color &color);

        void update();

        // CONSULTORS
        Color getColor() const;
        
        void draw();

    private:
        Color _color;

        shared_ptr<GameObject> _gameObject;
};
#endif