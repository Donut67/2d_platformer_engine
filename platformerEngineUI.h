#ifndef __PLATFORMER_ENGINE_UI_H__
#define __PLATFORMER_ENGINE_UI_H__

/*
#include "RuntimeInclude.h"
RUNTIME_MODIFIABLE_INCLUDE;
/*
#include "RuntimeSourceDependency.h"
RUNTIME_COMPILER_SOURCEDEPENDENCY;
// */

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

static bool _hovering;

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
        Rectangle getAreaRect() const { return Rectangle{ _position.x, _position.y, _size.x, _size.y }; }
        vector<shared_ptr<EngineObject>> object_list() const {return _object_list;}
};

class EngineBaseEditor : public EngineObject{
    private:
        shared_ptr<EngineObject> _screen, _floating;
        Scene _scene;
        Vector2 _prev_position, _prev_mouse_position;
        bool _dragging, _moving, _scene_loaded;
        string _scene_path;


        void openFileExplorer();
        void saveScene();
        void addFileOptions();
        void setHierarchy(bool state);
        void setInspector(bool state);
        void setAnimationEditor(bool state);
        void setTileSetEditor(bool state);

        void removeWindow(string name);
    public:
        EngineBaseEditor();

        void update();
        void draw();
        void setScene(string name);
};

class EngineColorWheel : public EngineObject {
    private:
        Color _current;
        bool _cancel_hover, _confirm_hover, _close;
        Font _font;

        function<void(Color)> _on_confirm, _on_cancel;
    public:
        EngineColorWheel() : EngineObject() {
            _cancel_hover = _confirm_hover = _close = false;
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _current = WHITE;
        }
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
        int _gap;
        bool _vertical;
    public:
        EngineFitter() : EngineObject() {
            _padding = Padding();
            _gap = 0;
            _vertical = true;
        }
        EngineFitter(Vector2 offset, Vector2 size, Padding padding, int gap, bool v = true) : EngineObject(offset, size), _padding(padding), _gap(gap), _vertical(v) {}

        void addObject(shared_ptr<EngineObject> obj);
        void removeObject(shared_ptr<EngineObject> obj);
        // void draw();
};

class EngineLabel : public EngineObject {
    private:
        string _text;
        Font _font;
        int _font_size;
        Padding _padding;
    public:
        EngineLabel() : EngineObject() {
            _font_size = 18;
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _padding = Padding();
        }
        EngineLabel(Vector2 offset, Vector2 size, string text, int fontSize) : EngineObject(offset, size), _text(text), _font_size(fontSize) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _padding = Padding();
        }
        EngineLabel(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding) : EngineObject(offset, size), _text(text), _font_size(fontSize), _padding(padding) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
        }
        
        void draw();
        
};

class EngineSprite : public EngineObject {
    private:
        shared_ptr<Texture2D> _texture;
        Color _color;
    public:
        float _scale;

        EngineSprite(Vector2 offset, Vector2 size, float scale, Color color) : EngineObject(offset, size), _scale(scale), _color(color) {}
        EngineSprite(Vector2 offset, Vector2 size, shared_ptr<Texture2D> texture, float scale) : EngineObject(offset, size), _texture(texture), _scale(scale) {_color = WHITE;}
        EngineSprite(Vector2 offset, Vector2 size, shared_ptr<Texture2D> texture, float scale, Color color) : EngineObject(offset, size), _texture(texture), _scale(scale), _color(color) {}

        void setTexture(shared_ptr<Texture2D> text);
        void setScale(float scale);
        void setColor(Color c);

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
        int _count;
    public:
        EngineButton() : EngineObject() {
            _font_size = 18;
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _padding = Padding();
            _hover = _right_click = false;
            _count = 10;
        }
        EngineButton(Vector2 offset, Vector2 size, string text, int fontSize, function<void()> on_click, bool fit = true) : EngineObject(offset, size), _text(text), _font_size(fontSize) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _padding = Padding();
            _hover = _right_click = false;
            _on_click = on_click;
            if (fit) _size.x = MeasureTextEx(_font, _text.c_str(), (float)_font_size, 0).x + _padding.left + _padding.right;
        }
        EngineButton(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding, bool fit = true) : EngineObject(offset, size), _text(text), _font_size(fontSize), _padding(padding) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _hover = _right_click = false;
            _on_click = [](){};
            if (fit) _size.x = MeasureTextEx(_font, _text.c_str(), (float)_font_size, 0).x + _padding.left + _padding.right;
        }
        EngineButton(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding, function<void()> on_click, bool fit = true) : EngineObject(offset, size), _text(text), _font_size(fontSize), _padding(padding) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _hover = _right_click = false;
            _on_click = on_click;
            if (fit) _size.x = MeasureTextEx(_font, _text.c_str(), (float)_font_size, 0).x + _padding.left + _padding.right;
        }
        EngineButton(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding, function<void()> on_click, function<void()> on_right_click, bool fit = true) : EngineObject(offset, size), _text(text), _font_size(fontSize), _padding(padding) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _hover = false;
            _on_click = on_click;
            _on_right_click = on_right_click;
            _right_click = true;
            if (fit) _size.x = MeasureTextEx(_font, _text.c_str(), (float)_font_size, 0).x + _padding.left + _padding.right;
        }

        virtual void setWidth(float x) {
            _size.x = x;
            int n = nCharsSpace(_font, _text.c_str(), (float)_font_size, 0, x);
            if (n < _text.length()) _text = _text.substr((size_t)0, (size_t)n - 4) + "...";
        }

        void update();
        void draw();
};

class EngineCheckBox : public EngineObject {
    private:
    public:
        function<void()> _on_select, _on_diselect;
        Font _font;
        bool _hover, _selected;
        EngineCheckBox(Vector2 offset, Vector2 size, function<void()> on_select, function<void()> on_diselect) : EngineObject(offset, size) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _hover = _selected = false;
            _on_select = on_select;
            _on_diselect = on_diselect;
        }

        void update();
        void draw();
};

class EngineToggleButton : public EngineCheckBox {
    private:
        string _text;
        Font _font;
        int _font_size;
        Padding _padding;
    public:
        EngineToggleButton(Vector2 offset, Vector2 size, string text, int fontSize, Padding padding, function<void()> on_select, function<void()> on_diselect) : EngineCheckBox(offset, size, on_select, on_diselect), _text(text), _font_size(fontSize) {
            _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
            _padding = padding;
            _size.x = MeasureTextEx(_font, _text.c_str(), (float)_font_size, 0).x + _padding.left + _padding.right;
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
        EngineColor() : EngineObject() {
            _color = WHITE;
        }
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
        int _initial_pos, _cursor_pos, _count;

        function<void(T)> _set_text;

        void updateText();
        void selectInput();
        void updateCursor();
    public:
        EngineInput() : EngineObject() {}
        EngineInput(Vector2 offset, Vector2 size, T item, function<void(T)> set_text);

        void update();
        void draw();
        void setItem(T item);
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

class EngineStackedItems : public EngineObject {
    private:
        Padding _padding;
        int _gap, _item_width, _item_height;
        bool _vertical, _set_width, _set_height;
    public:
        EngineStackedItems(Vector2 offset, Vector2 size, Padding padding, int gap, bool v = true);

        void addObject(shared_ptr<EngineObject> obj);
        void setItemWidth(int width);
        void setItemHeight(int height);

        Padding getPadding() const { return _padding; }
        void setWidth(int width);
        void setHeight(int height);
};

class EngineWindow : public EngineStackedItems {
    private:
        string _name;
        Vector2 _prev_position, _prev_mouse_position;
        bool _dragging, _moving;
    public:
        EngineWindow(string name, Vector2 offset, Vector2 size);

        void update();
        string getName() const;

        void draw() {
            EngineObject::draw();

            Rectangle r = getAreaRect();
            r.height = 0;
            for(auto item : _object_list) r.height += item->_size.y;

            DrawRectangleLinesEx(r, 1, Color{90, 90, 90, 255});
        }
};

class EngineOptionList : public EngineObject {
    private:
        function<void()> _close;
    public:
        EngineOptionList(const Vector2& pos, const Vector2& size, const Vector2& itemDimensions, function<void()> close);
        EngineOptionList(const Vector2& pos, const Vector2& size, const Vector2& itemDimensions, const vector<pair<string, function<void()>>>& items, function<void()> close);

        void addObject(shared_ptr<EngineObject> obj);
        void setWidth(int width);

        void update();
        void draw();
};

class EngineMenu : public EngineStackedItems {
    private:
        float _max_width;
        bool _opened;
        shared_ptr<EngineOptionList> _option_list;

        void click_button();
        void open();
        void close();
    public:
        EngineMenu(Vector2 offset, Vector2 size, string name);
        EngineMenu(Vector2 offset, Vector2 size, string name, const vector<pair<string, function<void()>>>& items);

        void addObject(shared_ptr<EngineObject> obj);

        void update();
        void draw() {
            EngineObject::draw();
            if (_opened) DrawRectangleLinesEx(_option_list->getAreaRect(), 1, Color{90, 90, 90, 255});
        }
};

class EngineHierarchy : public EngineSprite {
    private:
        static shared_ptr<EngineHierarchy> _instance;
        shared_ptr<Nary<GameObject>> _root;
        Nary<pair<bool, shared_ptr<GameObject>>> _hierarchyTree;
        shared_ptr<EngineObject> _screen, _option_list;
        bool _opened;

        void selectItem_i(Nary<pair<bool, shared_ptr<GameObject>>> tree, shared_ptr<GameObject> g);
        void addGameObject(shared_ptr<GameObject> go);
    protected:
        EngineHierarchy(Vector2 offset, Vector2 size, shared_ptr<Nary<GameObject>> root);
    public:
        EngineHierarchy(Vector2 offset, Vector2 size);
        static shared_ptr<EngineHierarchy> getInstance();
        
        void setRoot(shared_ptr<Nary<GameObject>> root);
        void selectItem(shared_ptr<GameObject> go);
        void openGameObjectOptions(shared_ptr<GameObject> go);
        void setBaseObject(shared_ptr<EngineObject> obj);
};

class EngineInspector : public EngineSprite {
    private:
        static shared_ptr<EngineInspector> _instance;
        shared_ptr<GameObject> _game_object;
        shared_ptr<EngineObject> _content;

        void openNewBehaviorOptions();

        template<typename N> 
        void addBehaviour(shared_ptr<N> behaviour){
            _game_object->addBehaviour(behaviour);
            setGameObject(_game_object);
        }
    protected:
        EngineInspector(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
    public:
        EngineInspector(Vector2 offset, Vector2 size);
        static shared_ptr<EngineInspector> getInstance();

        void setGameObject(shared_ptr<GameObject> go);
};

class EngineAnimationEditor : public EngineSprite {
    private:
        static shared_ptr<EngineAnimationEditor> _instance;

        shared_ptr<EngineObject> _options, _sequences, _selector, _sequence, _obj;

        void addNewAnimation();
        void selectTileSet();
        void setTexture(string path);
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

class EngineTileSetEditor : public EngineSprite {
    private:
        static shared_ptr<EngineTileSetEditor> _instance;

        shared_ptr<EngineObject> _sprite, _obj, _rows, _columns;
        shared_ptr<TileSetData> _data;

        void createTileSet();

        void selectTileSet();
        void selectTexture();

        void setTexture(string path);
        void setTileSet(string path);

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
        void setTexture(shared_ptr<Texture2D> texture);
        void draw();

        void setRows(int i);
        void setColumns(int i);
};

class EngineFileSelector : public EngineSprite {
    private:

    public:
        EngineFileSelector(Vector2 offset, Vector2 size, string path, string extension, function<void(string)> onclick);
};

class EngineGameObjectFacade : public EngineStackedItems {
    private:
        shared_ptr<GameObject> _game_object;
    public:
        EngineGameObjectFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

class EngineLabelFacade : public EngineStackedItems {
    private:
        shared_ptr<Label> _label;
    public:
        EngineLabelFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

class EngineSpriteFacade : public EngineStackedItems {
    private:
        shared_ptr<Sprite> _sprite;
    public:
        EngineSpriteFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

class EngineAnimatedSpriteFacade : public EngineStackedItems {
    private:
        shared_ptr<AnimatedSprite> _animated_sprite;
    public:
        EngineAnimatedSpriteFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

class EngineTileMapFacade : public EngineStackedItems {
    private:
        shared_ptr<TileMap> _tile_map;
    public:
        EngineTileMapFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go);
};

shared_ptr<EngineObject> CreateList(const Vector2 &offset, const Vector2 &itemDimensions, const vector<string> &items);
shared_ptr<EngineStackedItems> CreateInteractableList(const Vector2 &offset, const Vector2 &itemDimensions, const vector<pair<string, function<void()>>> &items);
shared_ptr<EngineObject> CreateHierarchyList(const Vector2 &offset, const Vector2 &itemDimensions, const HierarchyActionList &items);

Nary<pair<bool, shared_ptr<GameObject>>> convertToNewTree(Nary<GameObject> a);

HierarchyActionList NaryToActionList(Nary<pair<bool, shared_ptr<GameObject>>> h);
void NaryToActionList_i(const Nary<pair<bool, shared_ptr<GameObject>>> &a, HierarchyActionList &list, float offset);

void DebugGameObject(shared_ptr<GameObject> go);

#endif