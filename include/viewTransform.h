#ifndef __VIEW_TRANSFORM_H__
#define __VIEW_TRANSFORM_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
#include "textInput.h"

class ViewTransform : public Viewer, public Observer {
    private:
        Font _font;
        shared_ptr<TransformComp> _transform;
        TextInput<float> _x, _y, _width, _height;
        shared_ptr<GameObject> _go;
        
        Vector2 _pos;

    public: 
        ViewTransform() {
            _font = LoadFont("resources/monogram.ttf");
            _x = _y = _width = _height = TextInput<float>(Rectangle{0, 0, 100, 23});
        }
        void show();
        void update(shared_ptr<GameObject> go, const Vector2 &pos);
};
#endif