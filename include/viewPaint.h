#ifndef __VIEW_PAINT_H__
#define __VIEW_PAINT_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
#include "colorWheel.h"

class ViewPaint : public Viewer, public Observer {
    private:
        Font _font;
        shared_ptr<Paint> _paint;
        shared_ptr<ColorWheel> _cw;
        shared_ptr<GameObject> _go;
        
        Vector2 _pos;

    public: 
        ViewPaint() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(shared_ptr<GameObject> go, const Vector2 &pos);
};
#endif