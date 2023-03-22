#ifndef __TILE_SET_H__
#define __TILE_SET_H__

#include "raylib.h"
#include "myFunctions.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <memory>
#include <cstring>

using namespace std;

class TileSet{
    public:
        // CONSTRUCTORS
        TileSet(){}
        TileSet(shared_ptr<Texture2D> texture, string meta){
            _texture = texture;

            ifstream map("resources/" + meta);
            string line;
            int i = 0;

            while(getline(map, line)){
                vector<string> aux = splice(line, ' ');
                for(int j = 0; j < (int)aux.size(); j++){
                    Vector2 pos{(float)j, (float)i};
                    _map.insert(pair<int, Vector2>(stoi(aux[j]), pos));
                }
                i++;
            }
        }
        
        // DESTRUCTORS

        // MODIFIERS
        
        // CONSULTORS
        Vector2 operator [](const int &pos) const{
            map<int, Vector2>::const_iterator found = _map.find(pos);
            if(found != _map.end()) return found->second;
            return Vector2{-1.0f, -1.0f};
        }

        shared_ptr<Texture2D> getTexture() const{
            return _texture;
        }

    private:
        shared_ptr<Texture2D> _texture;
        map<int, Vector2> _map;
        Vector2 _tileSize;

};
#endif