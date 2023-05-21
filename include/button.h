#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class Button : public Component{
    public:
        // CONSTRUCTORS
        Button();
        Button(Color color);
        Button(Color color, Color hover);

        // DESTRUCTORS

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);
        void setColor(Color color);

        void update();

        // CONSULTORS
        bool isHover() const;
        bool isPressed() const;
        Color getColor() const;
        Color getHoverColor() const;
        
        void draw();

    private:
        bool _hover, _pressed;
        Color _color, _hovercolor;

        shared_ptr<GameObject> _gameObject;

};
#endif