#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include "tileset.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <utility>
#include <memory>

using namespace std;

class GameObject;
class Component;

class TileMap : public Component{
    public:
        // CONSTRUCTORS
        TileMap();
        TileMap(TileSet set, Vector2 size, float scale, string mapMeta);

        // DESTRUCTORS
        ~TileMap();

        // MODIFIERS
        void setGameObject(shared_ptr<GameObject> go);
        
        void update();

        // CONSULTORS
        string getFileName() const;
        string getMapMeta() const;
        float getScale() const;
        Vector2 getSize() const;
        Vector2 getTileSize() const;

        void draw();

    private:
        TileSet _set;
        Texture2D _texture;
        Vector2 _size, _tileSize;
        float _scale;
        string _filename, _mapMeta;

        map<pair<int, int>, int> _tileMap;

        shared_ptr<GameObject> _gameObject;
};
#endif