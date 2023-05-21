#ifndef __VIEW_BUTTON_H__
#define __VIEW_BUTTON_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
// #include "transform.h"

class ViewButton : public Viewer, public Observer {
    private:
        Font _font;
        
        Color _color, _hoverColor;
        shared_ptr<GameObject> _go;
        Vector2 _pos;

    public: 
        ViewButton() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(shared_ptr<GameObject> go, const Vector2 &pos);
};
#endif