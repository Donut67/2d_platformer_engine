#include "raylib.h"
#include "engineOptionList.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void EngineOptionList::update() {
    Window::update();
    for(auto i : _options) i->update();
}

void EngineOptionList::draw() {
    Window::draw();
    for(auto i : _options) i->draw();
}