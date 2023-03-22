#ifndef __LABEL_H__
#define __LABEL_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class GameObject;
class Component;

class Label : public Component{
    public:
        // CONSTRUCTORS
        Label();
        Label(string text, shared_ptr<Font> font, char allign, int fontSize, Color color);

        // DESTRUCTORS
        ~Label();

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);
        void setText(string text);
        void setColor(Color color);

        void update();

        // CONSULTORS
        string getText() const;
        char getAllign() const;
        int getfontSize() const;
        Color getColor() const;
        
        void draw();

    private:
        string _text;
        char _allign;
        int _fontSize;
        Color _color;
        shared_ptr<Font> _font;

        shared_ptr<GameObject> _gameObject;

};
#endif