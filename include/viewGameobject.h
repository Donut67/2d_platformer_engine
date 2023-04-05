#ifndef __VIEW_GAMEOBJECT_H__
#define __VIEW_GAMEOBJECT_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
// #include "transform.h"

class ViewGameobject : public Viewer, public Observer {
    private:
        Font _font;
        string _name;

        float _top;

    public: 
        ViewGameobject() {
            _font = LoadFont("resources/monogram.ttf");
        }
        void show();
        void update(const GameObject& go, const float &height);
};
#endif