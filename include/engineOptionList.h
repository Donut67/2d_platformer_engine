#ifndef __ENGINE_OPTION_LIST_H__
#define __ENGINE_OPTION_LIST_H__

#include "raylib.h"
#include "window.h"
#include "engineOption.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

class EngineOptionList : public Window {
    public:
        EngineOptionList(Vector2 pos, float width, float fontSize, vector<pair<string, function<void()>>> options) : Window("", pos, Vector2{width, options.size() * (fontSize + 2)}){
            int i = 0;
            for(auto item : options){
                _options.push_back(make_shared<EngineOption>(Vector2{pos.x, pos.y + (fontSize + 2) * i}, Vector2{width, fontSize + 2}, item.first, item.second));
                i++;
            }
        }

        void update();

        void draw();

    private:
        vector<shared_ptr<EngineOption>> _options;
};

#endif