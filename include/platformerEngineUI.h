#ifndef __PLATFORMER_ENGINE_UI_H__
#define __PLATFORMER_ENGINE_UI_H__

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <tuple>
#include "raylib.h"
#include "platformerEngineBasics.h"

using namespace std;

typedef vector<pair<pair<float, bool>, pair<string, tuple<function<void()>, function<void()>, function<void()>>>>> HierarchyActionList;

class EngineObject : public Object {
    protected:
    public:
        vector<shared_ptr<EngineObject>> _object_list;
        EngineObject() : Object() {}
        EngineObject(Vector2 offset, Vector2 size) : Object(offset, size) {}
        
        virtual void addObject(shared_ptr<EngineObject> obj);
        virtual void addTitleObject(const float &width, const string &title);
        virtual void addStackedObjects(const float &width, const vector<string> &texts);

        template<typename N> void addFacadeItem(const float &width, const string &title, const N& item);
        
        void update(const Vector2 &offset);
        void update();
        void draw();

        int getObjectListSize() const;
        vector<shared_ptr<EngineObject>> object_list() const {return _object_list;}
};

class EngineBaseEditor : public EngineObject{
    private:
        shared_ptr<EngineObject> _screen, _floating;
        Scene _scene;
        Vector2 _prev_position, _prev_mouse_position;
        bool _dragging, _moving;
    public:
        EngineBaseEditor() {}
        EngineBaseEditor(Scene scene);

        void update();
        void draw();
};

class EngineColorWheel : public EngineObject {
    private:
        Color _current;
        bool _cancel_hover, _confirm_hover, _close;
        Font _font;

        function<void(Color)> _on_confirm, _on_cancel;
    public:
        EngineColorWheel() : EngineObject() {}
        EngineColorWheel(Vector2 pos, Color current, function<void(Color)> onConfirm) : EngineObject(pos, Vector2{350.0f, 180.0f}), _current(current) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _on_confirm = onConfirm;
            _close = false;
        }

        void update();

        void draw();
        bool isClosed() const{
            return _close;
        }

};

class EngineFitter : public EngineObject {
    protected:
        Padding _padding;
        float _gap;
        bool _vertical;
    public:
        EngineFitter() : EngineObject() {}
        EngineFitter(Vector2 offset, Vector2 size, Padding padding, int gap, bool v = true) : EngineObject(offset, size), _padding(padding), _gap(gap), _vertical(v) {}
        
        void addObject(shared_ptr<EngineObject> obj);
        // void draw();
};

class EngineLabel : public EngineObject {
    private:
        string _text;
        Font _font;
        int _font_size;
        Padding _padding;
    public:
        EngineLabel() : EngineObject() {}
        EngineLabel(Vector2 offset, Vector2 size, string text, int fontSize) : EngineObject(offset, size), _text(text), _font_size(fontSize) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _padding = Padding{0, 0, 0, 0};
        }
        EngineLabel(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding) : EngineObject(offset, size), _text(text), _font_size(fontSize), _padding(padding) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
        }
        
        void draw();
        
};

class EngineSprite : public EngineObject {
    private:
        shared_ptr<Texture2D> _texture;
        float _scale;
        Color _color;
    public:
        EngineSprite(Vector2 offset, Vector2 size, float scale, Color color) : EngineObject(offset, size), _scale(scale), _color(color) {}
        EngineSprite(Vector2 offset, Vector2 size, shared_ptr<Texture2D> texture, float scale) : EngineObject(offset, size), _texture(texture), _scale(scale) {_color = WHITE;}
        EngineSprite(Vector2 offset, Vector2 size, shared_ptr<Texture2D> texture, float scale, Color color) : EngineObject(offset, size), _texture(texture), _scale(scale), _color(color) {}

        void setTexture(shared_ptr<Texture2D> text);
        void setScale(float scale);

        void draw();

        shared_ptr<Texture2D> texture() const {return _texture;}
        float scale() const {return _scale;}
};

class EngineButton : public EngineObject {
    private:
        function<void()> _on_click, _on_right_click;
        Font _font;
        string _text;
        int _font_size;
        Padding _padding;
        bool _hover, _right_click;
    public:
        EngineButton() : EngineObject() {}
        EngineButton(Vector2 offset, Vector2 size, string text, int fontSize, function<void()> on_click) : EngineObject(offset, size), _text(text), _font_size(fontSize) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _padding = Padding{0, 0, 0, 0};
            _hover = _right_click = false;
            _on_click = on_click;
        }
        EngineButton(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding) : EngineObject(offset, size), _text(text), _font_size(fontSize), _padding(padding) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _hover = _right_click = false;
            _on_click = [](){};
        }
        EngineButton(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding, function<void()> on_click) : EngineObject(offset, size), _text(text), _font_size(fontSize), _padding(padding) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _hover = _right_click = false;
            _on_click = on_click;
        }
        EngineButton(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding, function<void()> on_click, function<void()> on_right_click) : EngineObject(offset, size), _text(text), _font_size(fontSize), _padding(padding) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _hover = false;
            _on_click = on_click;
            _on_right_click = on_right_click;
            _right_click = true;
        }

        void update();
        void draw();
};

class EngineColor : public EngineObject {
    private:
        Color _color;

        function<void(Color)> _set_color;
        void setColor(Color c);
    public:
        EngineColor() : EngineObject() {}
        EngineColor(Vector2 offset, Vector2 size, Color color) : EngineObject(offset, size), _color(color) {}
        EngineColor(Vector2 offset, Vector2 size, Color color, function<void(Color)> set_color) : EngineObject(offset, size), _color(color), _set_color(set_color) {}

        void update();
        void draw();
};

template<class T>
class EngineInput : public EngineObject {
    private:
        T _item;
        string _text;
        Font _font;
        bool _hover, _selected, _decimal;
        int _initial_pos, _cursor_pos;

        function<void(T)> _set_text;

        void updateText();
        void selectInput();
        void updateCursor();
    public:
        EngineInput() : EngineObject() {}
        EngineInput(Vector2 offset, Vector2 size, T item, function<void(T)> set_text);

        void update();
        void draw();
};

class EngineAnimatedSpriteFrame : public EngineButton {
    private:
        shared_ptr<EngineAnimation> _actual;
    public:
        EngineAnimatedSpriteFrame(Vector2 offset, Vector2 size);

        void update();

        void setAnimation(shared_ptr<EngineAnimation> anim);
        void draw();
};

class EngineAux : public EngineSprite {
public:
    EngineAux(string name, Vector2 offset, Vector2 size) : EngineSprite(offset, size, 1, BLUE){
        addTitleObject(_size.x, "name");
    }

    void update() {
        // if (CheckCollisionPointRec(GetMousePosition(), getAreaRect()))
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            _position = GetMousePosition();
        _position.x += 5.0f;
        
        EngineObject::update();
    }
};

class EngineOptionList : public EngineFitter {
    // private:
    public:
        EngineOptionList() : EngineFitter() {}
        EngineOptionList(const Vector2 &pos, const Vector2 &size, const Vector2 &itemDimensions, const vector<pair<string, function<void()>>> &items);
};

class EngineHierarchy : public EngineFitter {
    private:
        static shared_ptr<EngineHierarchy> _instance;
        shared_ptr<Nary<GameObject>> _root;
        Nary<pair<bool, shared_ptr<GameObject>>> _hierarchyTree;

        void selectItem_i(Nary<pair<bool, shared_ptr<GameObject>>> tree, shared_ptr<GameObject> g);
        void addGameObject(shared_ptr<GameObject> go);
    protected:
        EngineHierarchy() : EngineFitter() {}
        EngineHierarchy(Vector2 offset, Vector2 size, shared_ptr<Nary<GameObject>> root);
    public:
        EngineHierarchy(Vector2 offset, Vector2 size);
        static shared_ptr<EngineHierarchy> getInstance();
        
        void setRoot(shared_ptr<Nary<GameObject>> root);
        void selectItem(shared_ptr<GameObject> go);
        void openGameObjectOptions(shared_ptr<GameObject> go);
};

class EngineInspector : public EngineFitter {
    private:
        static shared_ptr<EngineInspector> _instance;
        shared_ptr<GameObject> _game_object;

        void openNewBehaviorOptions();

        template<typename N> 
        void addBehaviour(shared_ptr<N> behaviour){
            _game_object->addBehaviour(behaviour);
            setGameObject(_game_object);
        }
    protected:
        EngineInspector() : EngineFitter() {}
        EngineInspector(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
    public:
        EngineInspector(Vector2 offset, Vector2 size);
        static shared_ptr<EngineInspector> getInstance();

        void setGameObject(shared_ptr<GameObject> go);
};

class EngineAnimationEditor : public EngineFitter {
    private:
        static shared_ptr<EngineAnimationEditor> _instance;

        shared_ptr<EngineObject> _options, _sequences, _selector, _sequence, _obj;

        void addNewAnimation();
        void selectTileSet();
        void setTexture(string path);
    protected:
        EngineAnimationEditor() : EngineFitter() {}
    public:
        EngineAnimationEditor(Vector2 offset, Vector2 size);
        static shared_ptr<EngineAnimationEditor> getInstance();

        void setBaseObject(shared_ptr<EngineObject> obj);
};

class EngineAnimationSelector : public EngineFitter {
    private:
        vector<shared_ptr<EngineAnimation>> _animation_list;
    public:
        EngineAnimationSelector(Vector2 offset, Vector2 size);

        void addAnimation(shared_ptr<EngineAnimation> anim, function<void()> select);
};

class EngineAnimationSequence : public EngineFitter {
    private:
        shared_ptr<EngineObject> _timers, _values;

        shared_ptr<EngineAnimation> _actual;
    public:
        EngineAnimationSequence(Vector2 offset, Vector2 size);

        void addNewPoint();
        void addPoint(int value, float time);
        void setAnimation(shared_ptr<EngineAnimation> anim);
        void setTexture(string path);
};

class EngineTileSetEditor : public EngineFitter {
    private:
        static shared_ptr<EngineTileSetEditor> _instance;

        shared_ptr<EngineObject> _sprite, _obj;
        shared_ptr<TileSetData> _data;

        void selectTexture();
        void setTexture(string path);
        void setRows(int i);
        void setColumns(int i);
        void saveData();

    public:
        EngineTileSetEditor(Vector2 offset, Vector2 size);
        static shared_ptr<EngineTileSetEditor> getInstance();

        void setBaseObject(shared_ptr<EngineObject> obj);
};

class EngineTileSetView : public EngineSprite {
    private:
        Vector2 _nTiles;
    public:
        EngineTileSetView(Vector2 offset, Vector2 size);

        void setTexture(string path);
        void draw();

        void setRows(int i);
        void setColumns(int i);
};

class EngineFileSelector : public EngineFitter {
    private:

    public:
        EngineFileSelector(Vector2 offset, Vector2 size, string path, string extension, function<void(string)> onclick);
};

class EngineGameObjectFacade : public EngineFitter {
    private:
        shared_ptr<GameObject> _game_object;
    public:
        EngineGameObjectFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

class EngineLabelFacade : public EngineFitter {
    private:
        shared_ptr<Label> _label;
    public:
        EngineLabelFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

class EngineSpriteFacade : public EngineFitter {
    private:
        shared_ptr<Sprite> _sprite;
    public:
        EngineSpriteFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

class EngineAnimatedSpriteFacade : public EngineFitter {
    private:
        shared_ptr<AnimatedSprite> _animated_sprite;
    public:
        EngineAnimatedSpriteFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

class EngineTileMapFacade : public EngineFitter {
    private:
        shared_ptr<TileMap> _tile_map;
    public:
        EngineTileMapFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

shared_ptr<EngineObject> CreateList(const Vector2 &offset, const Vector2 &itemDimensions, const vector<string> &items);
shared_ptr<EngineObject> CreateInteractableList(const Vector2 &offset, const Vector2 &itemDimensions, const vector<pair<string, function<void()>>> &items);
shared_ptr<EngineObject> CreateHierarchyList(const Vector2 &offset, const Vector2 &itemDimensions, const HierarchyActionList &items);

Nary<pair<bool, shared_ptr<GameObject>>> convertToNewTree(Nary<GameObject> a);

HierarchyActionList NaryToActionList(Nary<pair<bool, shared_ptr<GameObject>>> h);
void NaryToActionList_i(const Nary<pair<bool, shared_ptr<GameObject>>> &a, HierarchyActionList &list, float offset);

void DebugGameObject(shared_ptr<GameObject> go);

#endif