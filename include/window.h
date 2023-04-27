#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "raylib.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class Window {
    public:
        Window(string t, Vector2 p, Vector2 s) : _title(t), _position(p), _size(s){
            _font = LoadFont("resources/monogram.ttf");
            moving = false;
        }

        void update();

        void draw();
        bool isMoving() const;
        bool isClicked() const;

    protected:
        Vector2 _position, _size, _prevMousePosition;
        string _title;
        Font _font;
        bool moving, _isClicked;

};

#endif