#ifndef __VIEW_TRANSFORM_H__
#define __VIEW_TRANSFORM_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
// #include "transform.h"

class ViewTransform : public Viewer, public Observer {
    private:
        Font _font;
        Vector2 _position, _scale;
        
        Vector2 _pos;

    public: 
        ViewTransform() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(const GameObject& go, const Vector2 &pos);
};
#endif