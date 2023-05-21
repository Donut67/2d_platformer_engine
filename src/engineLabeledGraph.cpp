#include "raylib.h"
#include "engineLabeledGraph.h"
#include <cstdlib>
#include <iostream>
#include <string>

void WindowLabeledGraph::update() {
    Window::update();

    for(auto i : _states) {
        i->move(Window::movement());
        i->update();
        
        if(_destination.find(i->getTitle()) != _destination.end()) 
            for(auto j : _destination[i->getTitle()]) j->moveEnd(i->movement());
        
        if(_start.find(i->getTitle()) != _start.end()) 
            for(auto j : _start[i->getTitle()]) j->moveBegin(i->movement());
    }
}

void WindowLabeledGraph::draw() {
    Window::draw();

    for(auto i : _states) i->draw();
    for(auto i : _connections) i->draw();

}