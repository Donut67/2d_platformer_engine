#ifndef __VIEW_LABEL_H__
#define __VIEW_LABEL_H__

#include "viewer.h"
#include "subject.h"
#include "raylib.h"
#include "colorWheel.h"
#include "textInput.h"

class ViewLabel : public Viewer, public Observer {
    private:
        Font _font;
        shared_ptr<Label> _label;
        shared_ptr<ColorWheel> _cw;
        TextInput<string> _text;
        TextInput<int> _fontSize;
        shared_ptr<GameObject> _go;
        
        function<void()> _on_click;
        
        Vector2 _pos;

    public: 
        ViewLabel(function<void()> onClick) {
            _on_click = onClick;
            _font = LoadFont("resources/monogram.ttf");
            
            _text = TextInput<string>(Rectangle{0, 0, 250, 23});
            _fontSize = TextInput<int>(Rectangle{0, 0, 50, 23});
        }
        void show();
        void update(shared_ptr<GameObject> go, const Vector2 &pos);
};
#endif