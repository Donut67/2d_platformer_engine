#ifndef __ENGINE_OPTION_H__
#define __ENGINE_OPTION_H__

#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <functional>

using namespace std;

class EngineOption {
    public:
        EngineOption(Vector2 pos, Vector2 size, string name, function<void()> onClick) : _position(pos), _size(size), _name(name){
            _font = LoadFont("resources/monogram.ttf");
            _selected = _hovered = false;
            _on_click = onClick;
        }

        void update();
        void move(const Vector2 &movement);

        void draw();

    private:
        Vector2 _position, _size;
        string _name;
        Font _font;
        bool _selected, _hovered;

        function<void()> _on_click;
};

#endif