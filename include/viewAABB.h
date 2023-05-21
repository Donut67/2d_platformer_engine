#ifndef __VIEW_AABB_H__
#define __VIEW_AABB_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
// #include "transform.h"

class ViewAABB : public Viewer, public Observer {
    private:
        Font _font;
        shared_ptr<GameObject> _go;
        
        bool _colliding, _static, _transparent;
        Vector2 _pos;

    public: 
        ViewAABB() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(shared_ptr<GameObject> go, const Vector2 &pos);
};
#endif