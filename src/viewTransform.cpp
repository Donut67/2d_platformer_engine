#include "../include/viewTransform.h"

void ViewTransform::show(){
    // cout << "POSITION: " << _data.position.x << ", " << _data.position.y << "\n"; 
    // cout << "SIZE:     " << _data.scale.x << ", " << _data.scale.y << "\n"; 

    DrawRectangle(10, 10, 200, 100, LIGHTGRAY);
    DrawText(_data.name.c_str(), 20, 20, 28, WHITE);
}

void ViewTransform::update(const Data& data){
    _data = data;
    show();
}