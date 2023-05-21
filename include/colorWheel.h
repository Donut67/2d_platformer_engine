#ifndef __ENGINE_COLOR_WHEEL_H__
#define __ENGINE_COLOR_WHEEL_H__

#include "raylib.h"
#include "window.h"
#include <cstdlib>
#include <iostream>
#include <functional>

using namespace std;

class ColorWheel : public Window {
    public:
        ColorWheel() : Window("", Vector2{0,0}, Vector2{0,0}){}
        ColorWheel(Vector2 pos, Color current, function<void(Color)> onConfirm) : Window("Color Wheel", pos, Vector2{350.0f, 180.0f}), _current(current) {
            _on_confirm = onConfirm;
            _close = false;
        }

        void update();

        void draw();
        bool isClosed() const{
            return _close;
        }

    private:
        Color _current;
        bool _cancel_hover, _confirm_hover, _close;

        function<void(Color)> _on_confirm, _on_cancel;
};

#endif