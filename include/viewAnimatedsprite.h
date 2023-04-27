#ifndef __VIEW_ANIMATED_SPRITE_H__
#define __VIEW_ANIMATED_SPRITE_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
// #include "transform.h"

class ViewAnimatedSprite : public Viewer, public Observer {
    private:
        Font _font;
        
        string _filename;
        Vector2 _tileSize;
        float _scale;
        Vector2 _pos;

    public: 
        ViewAnimatedSprite() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(const GameObject& go, const Vector2 &pos);
};
#endif