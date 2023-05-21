#ifndef __ENGINE_STATE_WINDOW_H__
#define __ENGINE_STATE_WINDOW_H__

#include "raylib.h"
#include "window.h"
#include "engineOption.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <functional>

using namespace std;

typedef struct ConditionButtonData {
    string _option;
    function<void()> _on_click;
    function<void()> _on_select;

    ConditionButtonData() {}
    ConditionButtonData(string option, function<void()> on_click, function<void()> on_select) {
        _option = option;
        _on_click = on_click;
        _on_select = on_select;
    }
} ConditionButtonData;

class EngineStateWindow : public Window {
    public:
        EngineStateWindow(vector<ConditionButtonData> options, string t, Vector2 p, Vector2 s, int font_size, bool sticky = true) : Window(t, p, s, font_size, sticky){
            int gap = 25;
            for(ConditionButtonData i : options){
                _conditions.push_back(make_shared<EngineOption>(Vector2{p.x + 5, p.y + gap}, Vector2{s.x - 20, 20}, i._option, i._on_click));
                _anchor_points.push_back(make_shared<EngineOption>(Vector2{p.x + s.x - 15, p.y + gap}, Vector2{10, 20}, "", i._on_select));
                gap += 25;
            }
        }

        void update();

        void draw();

    private:
        vector<shared_ptr<EngineOption>> _conditions, _anchor_points;
};

#endif