#ifndef __VIEW_SPRITE_H__
#define __VIEW_SPRITE_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"

class ViewSprite : public Viewer, public Observer {
    private:
        Font _font;
        
        string _filename;
        shared_ptr<Texture2D> _texture;
        float _scale;
        shared_ptr<GameObject> _go;
        
        Vector2 _pos;

    public: 
        ViewSprite() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(shared_ptr<GameObject> go, const Vector2 &pos);
};
#endif