#ifndef __DATA_H__
#define __DATA_H__

#include <iostream> 
#include "raylib.h"

using namespace std;

struct Data {
    Vector2 position, scale;
    string name;

    friend ostream &operator<<(ostream &os, const Data &data) { 
        os << "POSITION: " << data.position.x << ", " << data.position.y << "\n"; 
        os << "SIZE:     " << data.scale.x << ", " << data.scale.y << "\n"; 
        return os; 
    }
};
#endif