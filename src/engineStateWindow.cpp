#include "raylib.h"
#include "engineStateWindow.h"
#include <cstdlib>
#include <iostream>
#include <string>

void EngineStateWindow::update() {
    Window::update();

    for(auto i : _conditions) {
        i->move(Window::movement());
        i->update();
    }
    
    for(auto i : _anchor_points) {
        i->move(Window::movement());
        i->update();
    }
}

void EngineStateWindow::draw() {
    Window::draw();

    for(auto i : _conditions) i->draw();
    for(auto i : _anchor_points) i->draw();
}