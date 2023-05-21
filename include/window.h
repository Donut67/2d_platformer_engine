#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "raylib.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class Window {
    public:
        Window(string t, Vector2 p, Vector2 s, bool sticky = true) : _title(t), _position(p), _size(s), _sticky(sticky){
            _font = LoadFont("resources/monogram.ttf");
            _dragging = _moving = false;
            _font_size = 28;
        }
        Window(string t, Vector2 p, Vector2 s, int font_size, bool sticky = true) : _title(t), _position(p), _size(s), _font_size(font_size), _sticky(sticky){
            _font = LoadFont("resources/monogram.ttf");
            _dragging = _moving = false;
        }

        void update();
        void move(const Vector2 &movement);

        void draw();
        Vector2 movement() const;
        bool isDragging() const;
        bool isClicked() const;
        string getTitle() const;
        Vector2 getPosition() const;

    protected:
        Vector2 _position, _prev_position, _size, _prevMousePosition;
        string _title;
        Font _font;
        bool _dragging, _moving, _isClicked, _sticky;
        int _font_size;

};

#endif