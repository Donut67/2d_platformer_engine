#ifndef __ENGINE_TEXT_INPUT_H__
#define __ENGINE_TEXT_INPUT_H__

#include "raylib.h"
#include "window.h"
#include "myFunctions.h"

#include <cstdlib>
#include <iostream>
#include <functional>

using namespace std;

template<class element>
class TextInput {
    public:
        TextInput() {}
        TextInput(Rectangle rec, element text) : _rec(rec), _text(text) {
            _close = false;
        }

        void update() {}

        void draw() {}

    private:
        Rectangle _rec;
        element _text;

        bool _close, _focused;
};

template <>
class TextInput<int> {
    public:
        TextInput() {}
        TextInput(Rectangle rec) : _rec(rec) {
            _font = LoadFont("resources/monogram.ttf");
            _focused = false;
        }

        void update() {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) _focused = CheckCollisionPointRec(GetMousePosition(), _rec);

            if(_focused) {
                int key = GetCharPressed();
                while (key > 0){
                    if ((key >= 32) && (key <= 125)) _text += (char)key;
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) _text = _text.substr(0, _text.size()-1);
            }
        }

        void setPosition(Vector2 pos) {
            _rec.x = pos.x;
            _rec.y = pos.y;
        }

        void setValue(int val) {
            _text = to_string(val);
        }

        int getValue() const {
            return _text.size() == 0? 0 : stoi(_text);
        }

        void draw() {
            DrawTextEx(_font, _text.c_str(), Vector2{_rec.x + 5.0f, _rec.y}, 24, 1, WHITE);
            DrawRectangleLines(_rec.x, _rec.y, _rec.width, _rec.height, _focused? BLUE : BLACK);
        }

    private:
        Rectangle _rec;
        string _text;
        Font _font;

        bool _focused;
};

template <>
class TextInput<float> {
    public:
        TextInput() {}
        TextInput(Rectangle rec) : _rec(rec) {
            _font = LoadFont("resources/monogram.ttf");
            _focused = false;
            _precision = 3;
            _decimal = true;
        }

        void update() {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) _focused = CheckCollisionPointRec(GetMousePosition(), _rec);

            if(_focused) {
                int key = GetCharPressed();
                while (key > 0){
                    if ((key >= 32) && (key <= 125)) {
                        _text += (char)key;
                        if(_decimal) _precision++;
                        if(key == KEY_PERIOD) _decimal = true;
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    _text = _text.substr(0, _text.size()-1);
                    if(_precision > 0) {
                        _precision--;
                        if(_precision == 0) _decimal = false;
                    }
                }
            }
        }

        void setPosition(Vector2 pos) {
            _rec.x = pos.x;
            _rec.y = pos.y;
        }

        void setValue(float val) {
            _text = ftostr(val, _precision);
        }

        float getValue() const {
            return _text.size() == 0? 0 : stof(_text);
        }

        void draw() {
            DrawTextEx(_font, _text.c_str(), Vector2{_rec.x + 5.0f, _rec.y}, 24, 1, WHITE);
            DrawRectangleLines(_rec.x, _rec.y, _rec.width, _rec.height, _focused? BLUE : BLACK);
        }

    private:
        Rectangle _rec;
        string _text;
        Font _font;
        int _precision;

        bool _focused, _decimal;
};

template <>
class TextInput<string> {
    public:
        TextInput() {}
        TextInput(Rectangle rec) : _rec(rec) {
            _font = LoadFont("resources/monogram.ttf");
            _focused = false;
            _capslock = false;
        }

        void update() {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) _focused = CheckCollisionPointRec(GetMousePosition(), _rec);

            if(_focused) {
                int key = GetCharPressed();
                while (key > 0){
                    if ((key >= 32) && (key <= 125)) _text += (char)key;
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) _text = _text.substr(0, _text.size()-1);
            }
        }

        void setPosition(Vector2 pos) {
            _rec.x = pos.x;
            _rec.y = pos.y;
        }

        void setValue(string val) {
            _text = val;
        }

        string getValue() const {
            return _text;
        }

        void draw() {
            DrawTextEx(_font, _text.c_str(), Vector2{_rec.x + 5.0f, _rec.y}, 24, 1, WHITE);
            DrawRectangleLines(_rec.x, _rec.y, _rec.width, _rec.height, _focused? BLUE : BLACK);
        }

    private:
        Rectangle _rec;
        string _text;
        Font _font;

        bool _focused, _capslock;
};

#endif