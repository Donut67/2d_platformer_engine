#ifndef __VIEW_GAMEOBJECT_H__
#define __VIEW_GAMEOBJECT_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
#include "textInput.h"

class ViewGameobject : public Viewer, public Observer {
    private:
        Font _font;
        bool _enable;
        TextInput<string> _name;

        Vector2 _pos;

    public: 
        ViewGameobject() {
            _font = LoadFont("resources/monogram.ttf");
            _name = TextInput<string>(Rectangle{0, 0, 250, 23});
        }
        void show();
        void update(shared_ptr<GameObject> go, const Vector2 &pos);
};
#endif