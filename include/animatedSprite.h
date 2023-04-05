#ifndef __ANIMATEDSPRITE_BOX_H__
#define __ANIMATEDSPRITE_BOX_H__

#include "raylib.h"
#include "gameObject.h"
#include "component.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <memory>

using namespace std;

class GameObject;
class Component;

class AnimatedSprite : public Component{
    public:
        // CONSTRUCTORS
        AnimatedSprite();
        AnimatedSprite(string filename, shared_ptr<Texture2D> texture, float scale, string mapMeta, float timeForFrame);

        // DESTRUCTOR
        ~AnimatedSprite();

        // MODIFIERS
        void setAnimation(int animation);
        void setFlipH(bool flip);
        void setFlipV(bool flip);

        void update();

        void setGameObject(shared_ptr<GameObject> go);

        // CONSULTORS
        bool ended() const;

        string getFileName() const{ return _filename; }
        string getMetaName() const{ return _metaname; }

        float getScale() const{ return _scale; }
        int getActual() const{ return _actual; }
        
        vector<int> getCicle() const{ return _cicle; }
        Vector2 getSize() const{ return _size; }
        Vector2 getTileSize() const{ return _tileSize; }

        // DRAW
        void draw();


    private:
        shared_ptr<Texture2D> _texture;
        Vector2 _size, _tileSize;
        float _scale, _timeForFrame, _frameTime;
        int _actual, _animation;
        bool _ended, _fliph, _flipv;
        string _filename, _metaname;

        vector<int> _cicle;
        map<int, pair<vector<int>, bool>> _animationMap;

        shared_ptr<GameObject> _gameObject;

};
#endif