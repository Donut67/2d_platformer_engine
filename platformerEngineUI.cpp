#include "platformerEngineUI.h"

/*
#include "RuntimeSourceDependency.h"
RUNTIME_COMPILER_SOURCEDEPENDENCY;
// */

using namespace std;
namespace fs = std::filesystem;

// [ENGINE OBJECT]
void EngineObject::addObject(shared_ptr<EngineObject> obj) {
    _object_list.push_back(obj);
}

void EngineObject::addTitleObject(const float &width, const string &title) {
    shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{width, 20.0f}, Padding(0, 0, -1, 0), -1);
    addObject(new_item);

    new_item->addObject(make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 20.0f}, 1, Color{30, 30, 30, 255}));
    new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 20.0f}, title, 18, Padding(10, 0)));
}

void EngineObject::addStackedObjects(const float &width, const vector<string> &texts) {
    shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{width, 20.0f}, Padding(0, -1, -1, 0), 0, false);
    addObject(new_item);

    float itemWidth = width / texts.size();

    for (string item : texts) {
        auto a = make_shared<EngineLabel>(Vector2{ 0.0f, 0.0f }, Vector2{ itemWidth, 20.0f }, item, 18, Padding(5, 0));
        new_item->addObject(a);
    }
}

template<typename N> void EngineObject::addFacadeItem(const float &width, const string &title, const N& item) {

}

void EngineObject::update(const Vector2 &offset) {
    Object::update(offset);
    update();
}

void EngineObject::update() {
    for(auto &object : _object_list) object->update(_position);
}

void EngineObject::draw() {
    for(auto object : _object_list) object->draw();
}

int EngineObject::getObjectListSize() const {
    return (int)_object_list.size();
}

// [ENGINE BASE EDITOR]
EngineBaseEditor::EngineBaseEditor() : EngineObject(Vector2{0.0f, 0.0f}, Vector2{0.0f, 0.0f}) {
    _dragging = _moving = _scene_loaded = false;

    shared_ptr<EngineObject> menu_bar = make_shared<EngineSprite>(Vector2{ 0.0f, 0.0f }, Vector2{ (float)GetScreenWidth(), 20.0f }, 1, Color{ 90, 90, 90, 255 });

    shared_ptr<EngineObject> new_item = make_shared<EngineStackedItems>(Vector2{ 0.0f, 0.0f }, Vector2{ (float)GetScreenWidth(), 20.0f }, Padding(0), 1, false);
    menu_bar->addObject(new_item);

    new_item->addObject(make_shared<EngineMenu>(Vector2{ 0.0f, 0.0f }, Vector2{ 60.0f, 20.0f }, "File", vector<pair<string, function<void()>>>{
        {"New Scene", []() { cout << "Creating new scene\n"; }},
        {"Open Scene", bind(&EngineBaseEditor::openFileExplorer, this)},
        {"Save", bind(&EngineBaseEditor::saveScene, this) }
    }));
    new_item->addObject(make_shared<EngineMenu>(Vector2{ 0.0f, 0.0f }, Vector2{ 60.0f, 20.0f }, "Edit", vector<pair<string, function<void()>>>{
        {"Copy", []() { cout << "Copy\n"; }},
        {"Paste", []() { cout << "Paste\n"; }},
        {"Undo", []() { cout << "Undo\n"; }},
        {"Redo", []() { cout << "Redo\n"; }}
    }));
    {
        shared_ptr< EngineMenu> menu_item = make_shared<EngineMenu>(Vector2{ 0.0f, 0.0f }, Vector2{ 60.0f, 20.0f }, "View");
        
        menu_item->addObject(make_shared<EngineToggleButton>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 20.0f }, "Hierarchy", 18, Padding(5, 0), bind(&EngineBaseEditor::setHierarchy, this, true), bind(&EngineBaseEditor::setHierarchy, this, false)));
        menu_item->addObject(make_shared<EngineToggleButton>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 20.0f }, "Inspector", 18, Padding(5, 0), bind(&EngineBaseEditor::setInspector, this, true), bind(&EngineBaseEditor::setInspector, this, false)));
        menu_item->addObject(make_shared<EngineToggleButton>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 20.0f }, "Animation Editor", 18, Padding(5, 0), bind(&EngineBaseEditor::setAnimationEditor, this, true), bind(&EngineBaseEditor::setAnimationEditor, this, false)));
        menu_item->addObject(make_shared<EngineToggleButton>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 20.0f }, "TileSet Editor", 18, Padding(5, 0), bind(&EngineBaseEditor::setTileSetEditor, this, true), bind(&EngineBaseEditor::setTileSetEditor, this, false)));

        new_item->addObject(menu_item);
    }

    _screen = make_shared<EngineObject>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 0.0f});
    _screen->addObject(menu_bar);
    _floating = make_shared<EngineObject>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 0.0f});
}

void EngineBaseEditor::update() {
    _hovering = false;

    if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()})) {
            _dragging = true;
            _prev_mouse_position = GetMousePosition();
        }
    }else if(IsMouseButtonReleased(MOUSE_MIDDLE_BUTTON)) _dragging = false;

    if(_dragging) {
        Vector2 move{_prev_mouse_position.x - GetMousePosition().x, _prev_mouse_position.y - GetMousePosition().y};
        _position = Vector2{_position.x - move.x, _position.y - move.y};
        _prev_mouse_position = GetMousePosition();
    }

    if (_scene_loaded) _scene.update(_position);
    _screen->update();
    _floating->update();

    // EngineObject::update();
}

void EngineBaseEditor::draw() {
    Vector2 origin{float(int(_position.x) % 50) - 50, float(int(_position.y) % 50) - 50};

    for(int i = (int)origin.x; i <= GetScreenWidth() + 100; i += 50) 
        DrawLine(i, (int)origin.y, i, (int)origin.y + GetScreenHeight() + 100, i == _position.x? GREEN : Color{255, 255, 255, 100});
    
    for(int i = (int)origin.y; i <= GetScreenWidth() + 100; i += 50) 
        DrawLine((int)origin.x, i, (int)origin.x + GetScreenWidth() + 100, i, i == _position.y? RED : Color{255, 255, 255, 100});

    if (_scene_loaded) _scene.draw();
    _floating->draw();
    _screen->draw();

    EngineObject::draw();
}

void EngineBaseEditor::openFileExplorer() {
    shared_ptr<EngineWindow> new_window = make_shared<EngineWindow>("File Explorer '.scn'", Vector2{ 0.0f, 50.0f }, Vector2{300.0f, 300.0f});
    _floating->addObject(new_window);

    using placeholders::_1;
    new_window->addObject(make_shared<EngineFileSelector>(
        Vector2{ 0.0f, 0.0f }, Vector2{ 300.0f, 300.0f }, "\\resources", ".scn",
        bind(&EngineBaseEditor::setScene, this, _1)
    ));
}

void EngineBaseEditor::saveScene() {

}

void EngineBaseEditor::setScene(string path) {
    _scene_loaded = true;
    _scene_path = path;
    _scene = Scene(_scene_path);

    EngineHierarchy::getInstance()->setRoot(_scene.getRoot());

    removeWindow("File Explorer '.scn'");
}

void EngineBaseEditor::setHierarchy(bool state) {
    if (state) {
        shared_ptr<EngineObject> hierarchy_window = make_shared<EngineWindow>("Hierarchy", Vector2{ 0.0f, 20.0f }, Vector2{ 350.0f, 20.0f });
        dynamic_pointer_cast<EngineWindow>(hierarchy_window)->setItemWidth(-1);
        hierarchy_window->addObject(EngineHierarchy::getInstance());
        _floating->addObject(hierarchy_window);
        EngineHierarchy::getInstance()->setBaseObject(_screen);
    }
    else removeWindow("Hierarchy");
}

void EngineBaseEditor::setInspector(bool state) {
    if (state) {
        shared_ptr<EngineObject> inspector_window = make_shared<EngineWindow>("Inspector", Vector2{ GetScreenWidth() - 350.0f, 20.0f }, Vector2{ 350.0f, 20.0f });
        dynamic_pointer_cast<EngineWindow>(inspector_window)->setItemWidth(-1);
        inspector_window->addObject(EngineInspector::getInstance());
        _floating->addObject(inspector_window);
    }
    else removeWindow("Inspector");
}

void EngineBaseEditor::setAnimationEditor(bool state) {
    if (state) {
        shared_ptr<EngineObject> animation_editor_window = make_shared<EngineWindow>("Animation Editor", Vector2{ 0.0f, GetScreenHeight() - 200.0f }, Vector2{ 650.0f, 20.0f });
        dynamic_pointer_cast<EngineWindow>(animation_editor_window)->setItemWidth(-1);
        animation_editor_window->addObject(EngineAnimationEditor::getInstance());
        _floating->addObject(animation_editor_window);
        EngineAnimationEditor::getInstance()->setBaseObject(_floating);
    }
    else removeWindow("Animation Editor");
}

void EngineBaseEditor::setTileSetEditor(bool state) {
    if (state) {
        shared_ptr<EngineObject> tileset_editor_window = make_shared<EngineWindow>("TileSet Editor", Vector2{ 850.0f, GetScreenHeight() - 500.0f }, Vector2{ 650.0f, 20.0f });
        dynamic_pointer_cast<EngineWindow>(tileset_editor_window)->setItemWidth(-1);
        tileset_editor_window->addObject(EngineTileSetEditor::getInstance());
        _floating->addObject(tileset_editor_window);
        EngineTileSetEditor::getInstance()->setBaseObject(_floating);
    } else removeWindow("TileSet Editor");
}

void EngineBaseEditor::removeWindow(string name) {
    int i = 0;
    bool found = false;
    while (i < (int)_floating->_object_list.size() && !found) {
        if (auto j = dynamic_pointer_cast<EngineWindow>(_floating->_object_list[i])) if (j->getName() == name) found = true;
        else i++;
    }

    if (found) _floating->_object_list.erase(_floating->_object_list.begin() + i);

}

// [ENGINE COLOR WHEEL]
void EngineColorWheel::update() {
    _cancel_hover = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 160, _position.y + 150, 80, 20});
    _confirm_hover = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 255, _position.y + 150, 80, 20});

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&_confirm_hover) {
        _on_confirm(_current);
        _close = true;
    }

    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 35, _size.x - 65.0f, 20}))
            _current.r = (unsigned char)((GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2));
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 57, _size.x - 65.0f, 20}))
            _current.g = (unsigned char)((GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2));
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 79, _size.x - 65.0f, 20}))
            _current.b = (unsigned char)((GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2));
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 101, _size.x - 65.0f, 20}))
            _current.a = (unsigned char)((GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2));
    }

    EngineObject::update();
}

void EngineColorWheel::draw() {
    float block_size = _size.x - 65.0f;

    DrawTextEx(_font, "R", Vector2{_position.x + 5, _position.y + 35}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 35, block_size, 20}, 2, BLACK);
    DrawRectangle((int)_position.x + 20 + (int)(_current.r / 255.0f * (block_size - 2)), (int)_position.y + 35, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.r).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 35}, 18, 0, BLACK);

    DrawTextEx(_font, "G", Vector2{_position.x + 5, _position.y + 57}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 57, block_size, 20}, 2, BLACK);
    DrawRectangle((int)_position.x + 20 + (int)(_current.g / 255.0f * (block_size - 2)), (int)_position.y + 57, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.g).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 57}, 18, 0, BLACK);

    DrawTextEx(_font, "B", Vector2{_position.x + 5, _position.y + 79}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 79, block_size, 20}, 2, BLACK);
    DrawRectangle((int)_position.x + 20 + (int)(_current.b / 255.0f * (block_size - 2)), (int)_position.y + 79, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.b).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 79}, 18, 0, BLACK);

    DrawTextEx(_font, "A", Vector2{_position.x + 5, _position.y + 101}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 101, block_size, 20}, 2, BLACK);
    DrawRectangle((int)_position.x + 20 + (int)(_current.a / 255.0f * (block_size - 2)), (int)_position.y + 101, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.a).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 101}, 18, 0, BLACK);

    DrawRectangleRec(Rectangle{_position.x + 20, _position.y + 123, block_size, 20}, _current);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 123, block_size, 20}, 2, BLACK);

    DrawRectangleRec(Rectangle{_position.x + 160, _position.y + 150, 80, 20}, _cancel_hover? BLUE : GRAY);
    DrawTextEx(_font, "CANCEL", Vector2{_position.x + 175, _position.y + 150}, 18, 0, BLACK);
    DrawRectangleRec(Rectangle{_position.x + 255, _position.y + 150, 80, 20}, _confirm_hover? BLUE : GRAY);
    DrawTextEx(_font, "CONFIRM", Vector2{_position.x + 265, _position.y + 150}, 18, 0, BLACK);

    EngineObject::draw();
}

EngineStackedItems::EngineStackedItems(Vector2 offset, Vector2 size, Padding padding, int gap, bool v) : EngineObject(offset, size), _padding(padding), _gap(gap), _vertical(v) {
    _item_width = _item_height = 0;
    _set_width = _set_height = false;
}

void EngineStackedItems::setItemWidth(int width) {
    _item_width = width;
    _set_width  = true;
}

void EngineStackedItems::setItemHeight(int height) {
    _item_height = height;
    _set_height  = true;
}

void EngineStackedItems::addObject(shared_ptr<EngineObject> obj) {
    EngineObject::addObject(obj);

    int n = (int)_object_list.size(), i = 0;
    float prev = 0.0f;

    for (shared_ptr<EngineObject> object : _object_list) {
        object->_offset.x = _padding.left;
        object->_offset.y = _padding.top; 
        
        if (_vertical) {
            object->_size.x = _set_width? _item_width == -1? _size.x - _padding.left - _padding.right : _item_width : object->_size.x;

            object->_offset.y += prev;
            prev += object->_size.y + _gap;
        } else {
            object->_size.y = _set_height ? _item_height == -1 ? _size.y - _padding.top - _padding.bottom : _item_height : object->_size.y;

            object->_offset.x += prev;
            prev += object->_size.x + _gap;
        }
    }
    if (_vertical) _size.y = prev;
    else _size.x = prev;
}

void EngineStackedItems::setWidth(int width) {
    for (shared_ptr<EngineObject> object : _object_list) object->setWidth((float)width);
    _size.x = (float)width;
}

void EngineStackedItems::setHeight(int height) {
    for (shared_ptr<EngineObject> object : _object_list) object->setHeight((float)height);
    _size.y = (float)height;
}

// [ENGINE FITTER]
void EngineFitter::addObject(shared_ptr<EngineObject> obj) {
    EngineObject::addObject(obj);

    int n = (int)_object_list.size(), i = 0;
    float prev = 0.0f;

    for(shared_ptr<EngineObject> object : _object_list){
        object->_offset.x = _padding.left != -1? _padding.left : _padding.right != -1? _size.x - object->_size.x - _padding.right : (_size.x - object->_size.x) / 2;
        object->_offset.y = _padding.top != -1? _padding.top : _padding.bottom != -1? _size.y - object->_size.y - _padding.bottom : (_size.y - object->_size.y) / 2;
        if(_gap != -1){
            if(_vertical) object->_offset.y += prev;
            else object->_offset.x += prev;
        }
        object->_size.x = _padding.left != -1 && _padding.right != -1? _size.x / (_gap != -1? n : 1) - _padding.left - _padding.right : object->_size.x;
        object->_size.y = _padding.top != -1 && _padding.bottom != -1? _size.y / (_gap != -1? n : 1) - _padding.top - _padding.bottom : object->_size.y;
        if(_gap != -1){
            if(_vertical) prev += object->_size.y + _gap;
            else prev += object->_size.x + _gap;
        }
        i++;
    }
    if(_gap != -1) {
        if(_vertical) _size.y = prev;
        else _size.x = prev;
    }
}

void EngineFitter::removeObject(shared_ptr<EngineObject> obj) {

}

// [ENGINE WINDOW]
EngineWindow::EngineWindow(string name, Vector2 offset, Vector2 size) : EngineStackedItems(offset, size, Padding(0), 0), _name(name) {
    _dragging = _moving = false;
    _prev_position = _prev_mouse_position = Vector2{ 0.0f, 0.0f };

    addTitleObject(_size.x, _name);
    EngineStackedItems::setItemWidth(-1);

}

void EngineWindow::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ _position.x, _position.y, _size.x, _size.y })) {
            _dragging = true;
            _prev_mouse_position = GetMousePosition();
        }
    } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) _dragging = false;

    if (_dragging) {
        Vector2 move{ _prev_mouse_position.x - GetMousePosition().x, _prev_mouse_position.y - GetMousePosition().y };
        _offset = Vector2{ _offset.x - move.x, _offset.y - move.y };
        _prev_mouse_position = GetMousePosition();
    }

    EngineObject::update();
}

string EngineWindow::getName() const {
    return _name;
}

// [ENGINE LABEL]
void EngineLabel::draw() {
    Rectangle rec{_position.x + _padding.left, _position.y + _padding.top, _size.x - _padding.left - _padding.right, _size.y - _padding.top - _padding.bottom};
    DrawTextEx(_font, _text.c_str(), Vector2{ _position.x + _padding.left, _position.y + _padding.top }, (float)_font_size, 0, WHITE);
    // DrawRectangleLines(_position.x + _padding.left, _position.y + _padding.top, _size.x - _padding.left - _padding.right, _size.y - _padding.top - _padding.bottom, BLUE);

    EngineObject::draw();
}

// [ENGINE SPRITE]
void EngineSprite::setTexture(shared_ptr<Texture2D> text) {
    _texture = text;
}

void EngineSprite::setScale(float scale) {
    _scale = scale;
}

void EngineSprite::setColor(Color c) {
    _color = c;
}

void EngineSprite::draw() {
    if(_texture != nullptr) DrawTextureEx(*_texture, _position, 0.0f, _scale, _color);
    else DrawRectangle((int)_position.x, (int)_position.y, (int)(_size.x * _scale), (int)(_size.y * _scale), _color);

    EngineObject::draw();
}

// [ENGINE ANIMATED SPRITE FRAME]
EngineAnimatedSpriteFrame::EngineAnimatedSpriteFrame(Vector2 offset, Vector2 size) : EngineButton(offset, size, "", 18, [](){cout << "Selecting Frame\n";}) {

}

void EngineAnimatedSpriteFrame::update() {
    

    EngineButton::update();
}

void EngineAnimatedSpriteFrame::setAnimation(shared_ptr<EngineAnimation> anim){
    _actual = anim;
}

void EngineAnimatedSpriteFrame::draw() {
    int value = _actual->_sequence[_actual->_actual].first;
    if((_actual->_data != nullptr && _actual->_data->_texture != nullptr) || value != -1) {
        int i = value % (int)(_actual->_data->_nTiles.x), j = (int)floor(value / (int)(_actual->_data->_nTiles.x));

        Rectangle src = Rectangle{i * _actual->_data->_tileSize.x, j * _actual->_data->_tileSize.y, _actual->_data->_tileSize.x, _actual->_data->_tileSize.y};
        Rectangle dst = Rectangle{_position.x, _position.y, _size.x, _size.y};

        DrawTexturePro(*_actual->_data->_texture, src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    }else DrawRectangle((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, Color{90, 90, 90, 255});
}

// [ENGINE BUTTON]
void EngineButton::update() {
    Rectangle rec{_position.x, _position.y, _size.x, _size.y - 1.0f};
    _hover = false;
    if(!_hovering) _hovering = _hover = CheckCollisionPointRec(GetMousePosition(), rec);

    if(_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) _on_click();
    if(_right_click && _hover && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) _on_right_click();

    EngineObject::update();
}

void EngineButton::draw() {
    Rectangle rec{_position.x + _padding.left, _position.y + _padding.top, _size.x - _padding.left - _padding.right, _size.y - _padding.top - _padding.bottom};
    DrawRectangle((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, _hover? BLUE : Color{30, 30, 30, 255});

    DrawTextEx(_font, _text.c_str(), Vector2{ _position.x + _padding.left, _position.y + _padding.top }, (float)_font_size, 0, WHITE);

    EngineObject::draw();
}

// [ENGINE CHECK BOX]
void EngineCheckBox::update() {
    Rectangle rec{ _position.x, _position.y, _size.x, _size.y - 1.0f };
    _hover = false;
    if(!_hovering) _hovering = _hover = CheckCollisionPointRec(GetMousePosition(), rec);

    if (_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        _selected = !_selected;
        if (_selected) _on_select();
        else _on_diselect();
    }

    EngineObject::update();
}

void EngineCheckBox::draw() {
    DrawRectangle((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, _selected ? BLUE : Color{ 30, 30, 30, 255 });
    DrawTextEx(_font, _selected ? "X" : " ", Vector2{ _position.x + 5.0f, _position.y }, _size.y, 0, WHITE);
    DrawRectangleLines((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, !_selected && _hover || _selected && !_hover ? BLUE : Color{ 30, 30, 30, 255 });
    EngineObject::draw();
}

// [ENGINE TOGGLE BUTTON]
void EngineToggleButton::update() {
    Rectangle rec{ _position.x, _position.y, _size.x, _size.y - 1.0f };
    _hover = false;
    if(!_hovering) _hovering = _hover = CheckCollisionPointRec(GetMousePosition(), rec);

    if (_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        _selected = !_selected;
        if (_selected) _on_select();
        else _on_diselect();
    }

    EngineObject::update();
}

void EngineToggleButton::draw() {
    DrawRectangle((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, _selected ? BLUE : Color{ 30, 30, 30, 255 });
    DrawTextEx(_font, _text.c_str(), Vector2{ _position.x + _padding.left, _position.y + _padding.top }, (float)_font_size, 0, WHITE);
    DrawRectangleLines((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, !_selected && _hover || _selected && !_hover ? BLUE : Color{ 30, 30, 30, 255 });

    EngineObject::draw();
}

// [ENGINE COLOR]
void EngineColor::setColor(Color c) {
    _object_list.clear();
    _set_color(c);
    _color = c;
}

void EngineColor::update() {
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x, _position.y, _size.x, _size.y}) && (int)_object_list.size() == 0) {

        using placeholders::_1;
        addObject(make_shared<EngineColorWheel>(Vector2{ -450.0f, 0.0f }, _color, bind(&EngineColor::setColor, this, _1)));
    }

    EngineObject::update();
}

void EngineColor::draw() {
    DrawRectangleRec(Rectangle{_position.x, _position.y, _size.x, _size.y}, Color{_color.r, _color.g, _color.b, 255});
    DrawRectangleRec(Rectangle{_position.x, _position.y + _size.y - 3.0f, _size.x, 3.0f}, RED);
    DrawRectangleRec(Rectangle{_position.x, _position.y + _size.y - 3.0f, _color.a / 255 * _size.x, 3.0f}, WHITE);

    DrawRectangleLines((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, BLACK);

    EngineObject::draw();
}

// [ENGINE INPUT]
template<class T>
void EngineInput<T>::selectInput() {
    _hover = false;
    if (!_hovering) _hovering = _hover = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x, _position.y, _size.x, _size.y});
    if(_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        _selected   = true;
        _cursor_pos = (int)_text.size();
    }
}

template<class T>
void EngineInput<T>::updateCursor() {
    if(_selected) {
        if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_BACKSPACE)) _cursor_pos -= _cursor_pos > 0? 1 : 0;
        else if(IsKeyPressed(KEY_RIGHT)) _cursor_pos += _cursor_pos < (int)_text.size()? 1 : 0;
    }
}

template<class T>
void EngineInput<T>::updateText() {
    selectInput();

    if(_selected) {
        int key = GetCharPressed();
        while (key > 0){
            if ((key >= 32) && (key <= 125)) {
                _text = _text.substr(0, _cursor_pos) + (char)key + _text.substr(_cursor_pos, _text.size());
                _cursor_pos += _cursor_pos < (int)_text.size()? 1 : 0;
            }
            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE)) _text = _text.substr(0, (size_t)_cursor_pos - 1) + ((int)_text.size() > _cursor_pos? _text.substr(_cursor_pos, _text.size()) : "");
        else if(IsKeyPressed(KEY_DELETE)) _text = _text.substr(0, _cursor_pos) + _text.substr((size_t)_cursor_pos + 1, _text.size());
    }

    updateCursor();
}

template<>
void EngineInput<int>::updateText() {
    selectInput();

    if(_selected) {
        int key = GetCharPressed();
        while (key > 0){
            if ((key >= 48) && (key <= 57)) {
                _text = _text.substr(0, _cursor_pos) + (char)key + _text.substr(_cursor_pos, _text.size());
                _cursor_pos += _cursor_pos < (int)_text.size()? 1 : 0;
            }
            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE)) _text = _text.substr(0, (size_t)_cursor_pos - 1) + ((int)_text.size() > _cursor_pos? _text.substr(_cursor_pos, _text.size()) : "");
        else if(IsKeyPressed(KEY_DELETE)) _text = _text.substr(0, _cursor_pos) + _text.substr((size_t)_cursor_pos + 1, _text.size());
    }

    updateCursor();
}

template<>
void EngineInput<float>::updateText() {
    selectInput();

    if(_selected) {
        int key = GetCharPressed();
        while (key > 0){
            if ((key >= 48 && key <= 57) || (key == 46 && !_decimal)) {
                _text = _text.substr(0, _cursor_pos) + (char)key + _text.substr(_cursor_pos, _text.size());
                _cursor_pos += _cursor_pos < (int)_text.size()? 1 : 0;
                if(!_decimal && key == 46) _decimal = true;
            }
            key = GetCharPressed();
        }

        if(IsKeyPressed(KEY_BACKSPACE)) {
            if(_decimal && _text[(size_t)_cursor_pos - 1] == '.') _decimal = false;
            _text = _text.substr(0, (size_t)_cursor_pos - 1) + ((int)_text.size() > _cursor_pos? _text.substr(_cursor_pos, _text.size()) : "");
        }else if(IsKeyPressed(KEY_DELETE)) {
            if(_decimal && _text[_cursor_pos] == '.') _decimal = false;
            _text = _text.substr(0, _cursor_pos) + _text.substr((size_t)_cursor_pos + 1, _text.size());
        }
    }

    updateCursor();
}
template<class T>
EngineInput<T>::EngineInput(Vector2 offset, Vector2 size, T item, function<void(T)> set_text) : EngineObject(offset, size), _item(item) {
    _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
    _selected = false;
    _text = _item;
    _initial_pos = 0;
    _set_text = set_text;
    _count = nCharsSpace(_font, _text, _size.y, 0, _size.x);
}

template<>
EngineInput<string>::EngineInput(Vector2 offset, Vector2 size, string item, function<void(string)> set_text) : EngineObject(offset, size), _item(item) {
    _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
    _selected = false;
    _text = _item;
    _initial_pos = 0;
    _set_text = set_text;
    _count = nCharsSpace(_font, _text, _size.y, 0, _size.x);
}

template<>
EngineInput<int>::EngineInput(Vector2 offset, Vector2 size, int item, function<void(int)> set_text) : EngineObject(offset, size), _item(item) {
    _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
    _selected = false;
    _text = to_string(_item);
    _initial_pos = 0;
    _set_text = set_text;
    _count = nCharsSpace(_font, _text, _size.y, 0, _size.x);
}

template<>
EngineInput<float>::EngineInput(Vector2 offset, Vector2 size, float item, function<void(float)> set_text) : EngineObject(offset, size), _item(item) {
    _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
    _selected = false;
    _text = ftostr(_item, 3);
    _initial_pos = 0;
    _set_text = set_text;
    _count = nCharsSpace(_font, _text, _size.y, 0, _size.x);
}

template<class T>
void EngineInput<T>::setItem(T item) {
    _item = item;
    _text = _item;
}

template<>
void EngineInput<string>::setItem(string item) {
    _item = item;
    _text = _item;
}

template<>
void EngineInput<int>::setItem(int item) {
    _item = item;
    _text = to_string(_item);
}

template<>
void EngineInput<float>::setItem(float item) {
    _item = item;
    _text = ftostr(_item, 3);
}

template<>
void EngineInput<string>::update() {
    updateText();

    if (_selected && IsKeyPressed(KEY_ENTER)) {
        _set_text(_text);
        _selected = false;
        _cursor_pos = -1;
    }

    EngineObject::update();
}

template<>
void EngineInput<int>::update() {
    updateText();

    if (_selected && IsKeyPressed(KEY_ENTER)) {
        _set_text(stoi(_text));
        _selected = false;
        _cursor_pos = -1;
    }

    EngineObject::update();
}

template<>
void EngineInput<float>::update() {
    updateText();

    if (_selected && IsKeyPressed(KEY_ENTER)) {
        _set_text(stof(_text));
        _selected = false;
        _cursor_pos = -1;
    }

    EngineObject::update();
}

template<class T>
void EngineInput<T>::draw() {
    Rectangle rec{ _position.x + 1, _position.y, _size.x, _size.y };

    int text_size = (int)_size.x / 8;

    if (_selected) _initial_pos = _cursor_pos - _initial_pos > text_size ? _initial_pos + 1 : _cursor_pos - _initial_pos < 0 ? _initial_pos - 1 : _initial_pos;

    string n_text = _text.substr(_initial_pos, _count);
    DrawTextEx(_font, n_text.c_str(), Vector2{ _position.x + 5, _position.y }, _size.y, 0, WHITE);

    if (_selected) DrawRectangle((_cursor_pos - _initial_pos) * 8 + (int)_position.x + 1, (int)_position.y + 17, 8, 2, BLUE);
    DrawRectangleLines((int)_position.x, (int)_position.y, (int)_size.x, (int)_size.y, _hover || _selected ? BLUE : Color{ 60, 60, 60, 255 });

    EngineObject::draw();
}

// [ENGINE OPTION LIST]
EngineOptionList::EngineOptionList(const Vector2& pos, const Vector2& size, const Vector2& itemDimensions, const vector<pair<string, function<void()>>>& items, function<void()> close) : EngineObject(pos, size) {
    EngineObject::addObject(CreateInteractableList(Vector2{ 0.0f, 0.0f }, itemDimensions, items));
    _size = _object_list[0]->_size;
    _close = close;
}

EngineOptionList::EngineOptionList(const Vector2& pos, const Vector2& size, const Vector2& itemDimensions, function<void()> close) : EngineObject(pos, size) {
    EngineObject::addObject(make_shared<EngineStackedItems>(Vector2{ 0.0f, 0.0f }, Vector2{ itemDimensions.x, 0.0f }, Padding(0), 0));
    _size = _object_list[0]->_size;
    _close = close;
}

void EngineOptionList::addObject(shared_ptr<EngineObject> obj) {
    _object_list[0]->addObject(obj);
}

void EngineOptionList::setWidth(int width) {
    _object_list[0]->setWidth(width);
}

void EngineOptionList::update() {
    if (CheckCollisionPointRec(GetMousePosition(), getAreaRect()) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) _close();

    EngineObject::update();
}

void EngineOptionList::draw() {
    EngineObject::draw();

    DrawRectangleLinesEx(getAreaRect(), 1, Color{90, 90, 90, 255});
}

// [ENGINE MENU]
EngineMenu::EngineMenu(Vector2 offset, Vector2 size, string name) : EngineStackedItems(offset, size, Padding(0), 0) {
    _max_width = 0.0f;
    _opened = false;
    _option_list = nullptr;
    addObject(make_shared<EngineButton>(Vector2{ 0.0f, 0.0f }, Vector2{ _size.x, 20.0f }, name, 18, Padding(15, 0), bind(&EngineMenu::click_button, this)));
    _option_list = make_shared<EngineOptionList>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 20.0f }, Vector2{ 100.0f, 20.0f }, bind(&EngineMenu::close, this));
}

EngineMenu::EngineMenu(Vector2 offset, Vector2 size, string name, const vector<pair<string, function<void()>>>& items) : EngineStackedItems(offset, size, Padding(0), 0) {
    _max_width = 0.0f; 
    _opened = false;
    _option_list = nullptr;
    addObject(make_shared<EngineButton>(Vector2{ 0.0f, 0.0f }, Vector2{ _size.x, 20.0f }, name, 18, Padding(15, 0), bind(&EngineMenu::click_button, this)));
    _option_list = make_shared<EngineOptionList>(Vector2{ 0.0f, 0.0f }, Vector2{ 100.0f, 20.0f }, Vector2{ 100.0f, 20.0f }, items, bind(&EngineMenu::close, this));
    cout << "okay\n";
}

void EngineMenu::addObject(shared_ptr<EngineObject> obj) {
    if (_option_list != nullptr && obj != _option_list) {
        _option_list->addObject(obj);
        _max_width = obj->_size.x > _max_width ? obj->_size.x : _max_width;

        dynamic_pointer_cast<EngineOptionList>(_option_list)->setWidth((int)_max_width);
    } else EngineStackedItems::addObject(obj);
}

void EngineMenu::click_button() {
    if (_opened) close();
    else open();
}

void EngineMenu::open() {
    if (!_opened) {
        _opened = true;
        addObject(_option_list);
    }
}

void EngineMenu::close() {
    if (_opened) {
        _opened = false;
        _object_list.pop_back();
    }
}

void EngineMenu::update() {
    if (_opened && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))) {
        if (!CheckCollisionPointRec(GetMousePosition(), Rectangle{ _option_list->_position.x, _option_list->_position.y, _option_list->_size.x, _option_list->_size.y })) close();
    }
    
    EngineObject::update();
}

// [ENGINE HIERARCHY]
shared_ptr<EngineHierarchy> EngineHierarchy::_instance = nullptr;
shared_ptr<EngineHierarchy> EngineHierarchy::getInstance() {
    if(_instance == nullptr) _instance = make_shared<EngineHierarchy>(Vector2{0.0f, 0.0f}, Vector2{350.0f, 0.0f});
    return _instance;
}

EngineHierarchy::EngineHierarchy(Vector2 offset, Vector2 size) : EngineSprite(offset, size, 1, Color{90, 90, 90, 255}) {
    _opened = false;
}

EngineHierarchy::EngineHierarchy(Vector2 offset, Vector2 size, shared_ptr<Nary<GameObject>> root) : EngineSprite(offset, size, 1, Color{ 90, 90, 90, 255 }) {
    setRoot(root);
}

void EngineHierarchy::setRoot(shared_ptr<Nary<GameObject>> root){
    _object_list.clear();
    _opened = false;
    _root = root;

    _hierarchyTree = convertToNewTree(*_root);

    addObject(CreateHierarchyList(Vector2{ 0.0f, 1.0f }, Vector2{ _size.x, 20.0f }, NaryToActionList(_hierarchyTree)));

    _size.y = _object_list[0]->_size.y;
}

void EngineHierarchy::selectItem(shared_ptr<GameObject> go){
    _object_list.clear();

    for(int i = 1; i <= _hierarchyTree.nBrothers(); i++) selectItem_i(_hierarchyTree.brother(i), go);

    addObject(CreateHierarchyList(Vector2{0.0f, 1.0f}, Vector2{_size.x, 20.0f}, NaryToActionList(_hierarchyTree)));

    _size.y = _object_list[0]->_size.y;
}

void EngineHierarchy::selectItem_i(Nary<pair<bool, shared_ptr<GameObject>>> a, shared_ptr<GameObject> g) {
    if(!a.isEmpty()) {
        if(g == a.content()->second) a.content()->first = !a.content()->first;
        if(a.content()->first) for(int i = 1; i <= a.nChilds(); i++) selectItem_i(a.child(i), g);
    }
}

void EngineHierarchy::openGameObjectOptions(shared_ptr<GameObject> go) {
    vector<pair<string, function<void()>>> action_list {
        {"New Game Object", bind(&EngineHierarchy::addGameObject, this, go)}
    };

    _option_list = make_shared<EngineOptionList>(GetMousePosition(), Vector2{ 250.0f, 20.0f }, Vector2{ 250.0f, 20.0f }, action_list, []() {});
    
    if (!_opened) {
        _screen->addObject(_option_list);
        _opened = true;
    }
}

void EngineHierarchy::setBaseObject(shared_ptr<EngineObject> obj) {
    _screen = obj;
}

void EngineHierarchy::addGameObject(shared_ptr<GameObject> go) {
    if (_opened) {
        cout << "Closing option list\n";
        _screen->_object_list.pop_back();
        _opened = false;
    }

    Nary<GameObject> result;
    
    shared_ptr<GameObject> object = make_shared<GameObject>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 0.0f}, "New Game Object");
    object->setTree(_root);
    result = Nary<GameObject>(object);

    _root->find(go).setChild(result);
    setRoot(_root);
}

// [ENGINE INSPECTOR]
shared_ptr<EngineInspector>EngineInspector::_instance = nullptr;
shared_ptr<EngineInspector>EngineInspector::getInstance() {
    if(_instance == nullptr) _instance = make_shared<EngineInspector>(Vector2{0.0f, 0.0f}, Vector2{350.0f, 0.0f});
    return _instance;
}

EngineInspector::EngineInspector(Vector2 offset, Vector2 size) : EngineSprite(offset, size, 1, Color{90, 90, 90, 255}) {
}

EngineInspector::EngineInspector(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineSprite(offset, size, 1, Color{ 90, 90, 90, 255 }) {
    setGameObject(go);
}

void EngineInspector::setGameObject(shared_ptr<GameObject> go) {
    _object_list.clear();
    _game_object = go;
    _size.y = 0.0f;

    _content = make_shared<EngineStackedItems>(Vector2{ 0.0f, 0.0f }, Vector2{_size.x, 0.0f}, Padding(0, 0, 5, 0), 1);
    dynamic_pointer_cast<EngineStackedItems>(_content)->setItemWidth(-1);

    if(_game_object != nullptr) {
        _content->addObject(make_shared<EngineGameObjectFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));

        // /*
        for(auto item : _game_object->getBehaviours()){
            if(dynamic_pointer_cast<Label>(item) != nullptr) _content->addObject(make_shared<EngineLabelFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));
            else if(dynamic_pointer_cast<Sprite>(item) != nullptr) _content->addObject(make_shared<EngineSpriteFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));
            else if(dynamic_pointer_cast<AnimatedSprite>(item) != nullptr) _content->addObject(make_shared<EngineAnimatedSpriteFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));
            else if(dynamic_pointer_cast<TileMap>(item) != nullptr) _content->addObject(make_shared<EngineTileMapFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));
        }
        // */

        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(-1, 0), 0, false);
        _content->addObject(new_item);

        new_item->addObject(make_shared<EngineButton>(Vector2{ 0.0f, 0.0f }, Vector2{ _size.x / 4.0f * 3.0f, 20.0f }, "Add Behaviour", 18, Padding(30, 0), bind(&EngineInspector::openNewBehaviorOptions, this)));
    }

    addObject(_content);
    _size.y += _content->_size.y + dynamic_pointer_cast<EngineStackedItems>(_content)->getPadding().top + dynamic_pointer_cast<EngineStackedItems>(_content)->getPadding().bottom;
}

void EngineInspector::openNewBehaviorOptions(){
    vector<pair<string, function<void()>>> action_list{
        {"New Label", bind(&EngineInspector::addBehaviour<Label>, this, make_shared<Label>("new label", 18))},
        {"New Sprite", bind(&EngineInspector::addBehaviour<Sprite>, this, make_shared<Sprite>(WHITE))},
        {"New Animated Sprite", bind(&EngineInspector::addBehaviour<AnimatedSprite>, this, make_shared<AnimatedSprite>())},
        {"New Tile Map", bind(&EngineInspector::addBehaviour<TileMap>, this, make_shared<TileMap>())}
    };

    shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{ 0.0f, 0.0f }, Vector2{ _size.x, 20.0f }, Padding(-1, 0), 0);
    _content->addObject(new_item);

    new_item->addObject(make_shared<EngineSprite>(Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 5.0f }, 1, Color{ 0, 0, 0, 0 }));
    new_item->addObject(CreateInteractableList(Vector2{ 0.0f, 10.0f }, Vector2{ 250.0f, 20.0f }, action_list));
}

// [ENGINE ANIMATION EDITOR]
shared_ptr<EngineAnimationEditor>EngineAnimationEditor::_instance = nullptr;
shared_ptr<EngineAnimationEditor>EngineAnimationEditor::getInstance() {
    if(_instance == nullptr) _instance = make_shared<EngineAnimationEditor>(Vector2{0.0f, 0.0f}, Vector2{800.0f, 0.0f});
    return _instance;
}

EngineAnimationEditor::EngineAnimationEditor(Vector2 offset, Vector2 size) : EngineSprite(offset, size, 1, Color{90, 90, 90, 255}) {
    shared_ptr<EngineObject> new_item = make_shared<EngineStackedItems>(Vector2{ 0.0f, 0.0f }, _size, Padding(0, 1), 1);
    addObject(new_item);

    _options = make_shared<EngineStackedItems>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0), 1, false);
    new_item->addObject(_options);

    _selector = make_shared<EngineAnimationSelector>(Vector2{ 0.0f, 0.0f }, Vector2{ 199.f, 100.0f });
    _sequence = make_shared<EngineAnimationSequence>(Vector2{ 0.0f, 0.0f }, Vector2{ _size.x - 201.f, 100.0f });

    _options->addObject(make_shared<EngineButton>(Vector2{ 0.0f, 0.0f }, Vector2{ 120.f, 20.0f }, "New Animation", 18, Padding(5, 0), bind(&EngineAnimationEditor::addNewAnimation, this)));
    _options->addObject(make_shared<EngineButton>(Vector2{ 0.0f, 0.0f }, Vector2{  90.f, 20.0f }, "New Point", 18, Padding(5, 0), bind(&EngineAnimationSequence::addNewPoint, dynamic_pointer_cast<EngineAnimationSequence>(_sequence))));
    _options->addObject(make_shared<EngineButton>(Vector2{ 0.0f, 0.0f }, Vector2{ 125.f, 20.0f }, "Select TileSet", 18, Padding(5, 0), bind(&EngineAnimationEditor::selectTileSet, this)));

    _sequences = make_shared<EngineStackedItems>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 100.0f}, Padding(0), 1, false);
    new_item->addObject(_sequences);
    
    _sequences->addObject(_selector);
    _sequences->addObject(_sequence);

    _size.y = new_item->_size.y;
}

void EngineAnimationEditor::addNewAnimation() {
    shared_ptr<EngineAnimation> actual = make_shared<EngineAnimation>(EngineAnimation{"New Animation", {}, false, 0, 0.0f});
    dynamic_pointer_cast<EngineAnimationSequence>(_sequence)->setAnimation(actual);

    using placeholders::_1;
    dynamic_pointer_cast<EngineAnimationSelector>(_selector)->addAnimation(
        actual, 
        bind(&EngineAnimationSequence::setAnimation, dynamic_pointer_cast<EngineAnimationSequence>(_sequence), actual)
    );
}

void EngineAnimationEditor::selectTileSet() {
    if (auto a = dynamic_pointer_cast<EngineAnimationSequence>(_sequence)) {
        shared_ptr<EngineWindow> new_window = make_shared<EngineWindow>("File Explorer '.tsd'", Vector2{ 0.0f, 50.0f }, Vector2{ 300.0f, 300.0f });
        _obj->addObject(new_window);

        using placeholders::_1;
        new_window->addObject(make_shared<EngineFileSelector>(
            Vector2{ 0.0f, 0.0f }, Vector2{ 300.0f, 300.0f }, "\\resources", ".tsd",
            bind(&EngineAnimationEditor::setTexture, this, _1)
        ));
    }
}

void EngineAnimationEditor::setTexture(string path) {
    dynamic_pointer_cast<EngineAnimationSequence>(_sequence)->setTexture(path);
    cout << "okay\n";
    
    int i = 0;
    bool found = false;
    while(i < (int)_obj->_object_list.size() && !found) {
        if(auto j = dynamic_pointer_cast<EngineWindow>(_obj->_object_list[i])) if (j->getName() == "File Explorer '.tsd'") found = true;
        else i++;
    }

    if(found) _obj->_object_list.erase(_obj->_object_list.begin()+i);
}

void EngineAnimationEditor::setBaseObject(shared_ptr<EngineObject> obj) {
    _obj = obj;
}

// [ENGINE ANIMATION SELECTOR]
EngineAnimationSelector::EngineAnimationSelector(Vector2 offset, Vector2 size) : EngineFitter(offset, size, Padding(0, -1, -1, 0), 1) {
    
}

void EngineAnimationSelector::addAnimation(shared_ptr<EngineAnimation> anim, function<void()> select){
    _animation_list.push_back(anim);

    addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, anim->_name, 18, Padding(5, 0), select));
}

// [ENGINE ANIMATION SEQUENCE]
EngineAnimationSequence::EngineAnimationSequence(Vector2 offset, Vector2 size) : EngineFitter(offset, size, Padding(0, 0, -1, 0), 1) {
    _actual = nullptr;

    _timers = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 1, false);
    addObject(_timers);

    _values = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 60.0f}, Padding(0, -1, -1, 0), 1, false);
    addObject(_values);
}

void EngineAnimationSequence::addNewPoint() {
    _actual->addPoint(-1, 0.5f);

    addPoint(-1, 0.5f);
}

void EngineAnimationSequence::addPoint(int value, float time) {
    if(_actual != nullptr) {
        using placeholders::_1;

        _timers->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{60.0f, 20.0f}, time, bind(&EngineAnimation::setTime, _actual, _timers->getObjectListSize(), _1)));
        shared_ptr<EngineObject> new_item = make_shared<EngineAnimatedSpriteFrame>(Vector2{0.0f, 0.0f}, Vector2{60.0f, 60.0f});
        dynamic_pointer_cast<EngineAnimatedSpriteFrame>(new_item)->setAnimation(_actual);
        _values->addObject(new_item);
    }
}

void EngineAnimationSequence::setAnimation(shared_ptr<EngineAnimation> anim){
    _object_list.clear();
    _actual = anim;

    _timers = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 1, false);
    addObject(_timers);

    _values = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 60.0f}, Padding(0, -1, -1, 0), 1, false);
    addObject(_values);

    for(auto item : _actual->_sequence) addPoint(item.first, item.second);
}

void EngineAnimationSequence::setTexture(string path) {
    _actual->setData(Resources::getInstance()->tileset(path, true));
}

// [ENGINE TILE SET EDITOR]
shared_ptr<EngineTileSetEditor>EngineTileSetEditor::_instance = nullptr;
shared_ptr<EngineTileSetEditor>EngineTileSetEditor::getInstance() {
    if(_instance == nullptr) _instance = make_shared<EngineTileSetEditor>(Vector2{0.0f, 0.0f}, Vector2{650.0f, 40.0f});
    return _instance;
}

EngineTileSetEditor::EngineTileSetEditor(Vector2 offset, Vector2 size) : EngineSprite(offset, size, 1, Color{90, 90, 90, 255}) {
    _data = nullptr;
    // /*
    shared_ptr<EngineObject> menu_bar = make_shared<EngineSprite>(Vector2{ 0.0f, 0.0f }, Vector2{ _size.x, 20.0f }, 1, Color{ 60, 60, 60, 255 });

    shared_ptr<EngineObject> options  = make_shared<EngineStackedItems>(Vector2{ 0.0f, 0.0f }, Vector2{ _size.x, 20.0f }, Padding(2, 0), 2, false);
    menu_bar->addObject(options);
    // /*
    options->addObject(make_shared<EngineMenu>(Vector2{ 0.0f, 0.0f }, Vector2{ 60.0f, 20.0f }, "File", vector<pair<string, function<void()>>>{
        { "New TileSet", bind(&EngineTileSetEditor::createTileSet, this) },
        { "Open TileSet", bind(&EngineTileSetEditor::selectTileSet, this) },
        { "Open Texture", bind(&EngineTileSetEditor::selectTexture, this) },
        { "Save", bind(&EngineTileSetEditor::saveData, this) }
    }));

    using placeholders::_1;
    options->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{55.0f, 20.0f}, " Rows:", 18, Padding(5, 0)));
    _rows = make_shared<EngineInput<int>>(Vector2{ 0.0f, 0.0f }, Vector2{ 20.0f, 20.0f }, 1, bind(&EngineTileSetEditor::setRows, this, _1));
    options->addObject(_rows);
    options->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{80.0f, 20.0f}, " Columns:", 18, Padding(5, 0)));
    _columns = make_shared<EngineInput<int>>(Vector2{ 0.0f, 0.0f }, Vector2{ 20.0f, 20.0f }, 1, bind(&EngineTileSetEditor::setColumns, this, _1));
    options->addObject(_columns);


    _sprite = make_shared<EngineTileSetView>(Vector2{0.0f, 20.0f}, Vector2{_size.x, 400.0f});
    addObject(_sprite);
    addObject(menu_bar);
    // */
}

void EngineTileSetEditor::createTileSet() {
    cout << "Creating a new tile set\n";
}

void EngineTileSetEditor::selectTileSet() {
    shared_ptr<EngineWindow> new_window = make_shared<EngineWindow>("File Explorer '.tsd'", Vector2{ 0.0f, 50.0f }, Vector2{ 300.0f, 300.0f });
    _obj->addObject(new_window);

    using placeholders::_1;
    new_window->addObject(make_shared<EngineFileSelector>(
        Vector2{ 0.0f, 0.0f }, Vector2{ 300.0f, 300.0f }, "\\resources", ".tsd",
        bind(&EngineTileSetEditor::setTileSet, this, _1)
    ));
}

void EngineTileSetEditor::setTileSet(string path) {
    _data = make_shared<TileSetData>(path);
    dynamic_pointer_cast<EngineTileSetView>(_sprite)->setTexture(_data->_texture);

    dynamic_pointer_cast<EngineInput<int>>(_rows)->setItem((int)_data->_nTiles.y);
    dynamic_pointer_cast<EngineInput<int>>(_columns)->setItem((int)_data->_nTiles.x);

    dynamic_pointer_cast<EngineTileSetView>(_sprite)->setRows((int)_data->_nTiles.y);
    dynamic_pointer_cast<EngineTileSetView>(_sprite)->setColumns((int)_data->_nTiles.x);

    _size.y = dynamic_pointer_cast<EngineTileSetView>(_sprite)->_size.y;

    int i = 0;
    bool found = false;
    while (i < (int)_obj->_object_list.size() && !found) {
        if (auto j = dynamic_pointer_cast<EngineWindow>(_obj->_object_list[i])) if (j->getName() == "File Explorer '.tsd'") found = true;
        else i++;
    }

    if (found) _obj->_object_list.erase(_obj->_object_list.begin() + i);
}

void EngineTileSetEditor::selectTexture() {
    shared_ptr<EngineWindow> new_window = make_shared<EngineWindow>("File Explorer '.png'", Vector2{ 0.0f, 50.0f }, Vector2{ 300.0f, 300.0f });
    _obj->addObject(new_window);

    using placeholders::_1;
    new_window->addObject(make_shared<EngineFileSelector>(
        Vector2{ 0.0f, 0.0f }, Vector2{ 300.0f, 300.0f }, "\\resources", ".png",
        bind(&EngineTileSetEditor::setTexture, this, _1)
    ));
}

void EngineTileSetEditor::setTexture(string path) {
    dynamic_pointer_cast<EngineTileSetView>(_sprite)->setTexture(path);
    if(_data == nullptr) _data = make_shared<TileSetData>();
    _data->_texture = Resources::getInstance()->texture(path, true);

    _size.y = dynamic_pointer_cast<EngineTileSetView>(_sprite)->_size.y;

    int i = 0;
    bool found = false;
    while (i < (int)_obj->_object_list.size() && !found) {
        if (auto j = dynamic_pointer_cast<EngineWindow>(_obj->_object_list[i])) if (j->getName() == "File Explorer '.png'") found = true;
        else i++;
    }

    // if (found) _obj->_object_list.erase(_obj->_object_list.begin() + i);
}

void EngineTileSetEditor::setRows(int i) {
    dynamic_pointer_cast<EngineTileSetView>(_sprite)->setRows(i);
    _data->setRows(i);
}

void EngineTileSetEditor::setColumns(int i) {
    dynamic_pointer_cast<EngineTileSetView>(_sprite)->setColumns(i);
    _data->setColumns(i);
}

void EngineTileSetEditor::saveData() {
    if(_data != nullptr) {
        string data = _data->_path + " " + to_string((int)_data->_nTiles.x) + " " + to_string((int)_data->_nTiles.y) + " " + to_string((int)_data->_tileSize.x) + " " + to_string((int)_data->_tileSize.y);
        string extension = ".tsd";

        ofstream file(_data->_path.substr(0, _data->_path.find('.')) + extension);
        file << data;
        file.close();
    }
}

void EngineTileSetEditor::setBaseObject(shared_ptr<EngineObject> obj) {
    _obj = obj;
}

// [ENGINE TILE SET VIEW]
EngineTileSetView::EngineTileSetView(Vector2 offset, Vector2 size) : EngineSprite(offset, size, 1, Color{90, 90, 90, 255}) {
    _nTiles = Vector2{1.0f, 1.0f};
}

void EngineTileSetView::setTexture(string path) {
    shared_ptr<Texture2D> text = Resources::getInstance()->texture(path, true);

    EngineSprite::setTexture(text);
    EngineSprite::setScale(_size.x / text->width);

    _size.y = EngineSprite::texture()->height * EngineSprite::scale() + 20.0f;
}

void EngineTileSetView::setTexture(shared_ptr<Texture2D> texture) {
    EngineSprite::setTexture(texture);
    EngineSprite::setScale(_size.x / texture->width);

    _size.y = EngineSprite::texture()->height * EngineSprite::scale() + 20.0f;
}

void EngineTileSetView::draw() {
    EngineSprite::draw();

    if(EngineSprite::texture() != nullptr) {
        for(int i = 1; i < _nTiles.x; i++) 
            DrawLine((int)(_position.x + _size.x / _nTiles.x * i), (int)_position.y, (int)(_position.x + _size.x / _nTiles.x * i), (int)(_position.y + EngineSprite::texture()->height * EngineSprite::scale()), BLUE);
        for(int i = 1; i < _nTiles.y; i++) 
            DrawLine((int)_position.x, (int)(_position.y + EngineSprite::texture()->height * EngineSprite::scale() / _nTiles.y * i), (int)(_position.x + _size.x), (int)(_position.y + EngineSprite::texture()->height * EngineSprite::scale() / _nTiles.y * i), BLUE);
        DrawRectangleLines((int)_position.x, (int)_position.y, (int)_size.x, (int)(EngineSprite::texture()->height * EngineSprite::scale()), BLUE);
    }
}

void EngineTileSetView::setRows(int i){
    _nTiles.y = (float)i;
}

void EngineTileSetView::setColumns(int i){
    _nTiles.x = (float)i;
}

// [ENGINE FILE SELECTOR]
EngineFileSelector::EngineFileSelector(Vector2 offset, Vector2 size, string path, string extension, function<void(string)> onclick) : EngineSprite(offset, size, 1, Color{ 90, 90, 90, 255 }) {
    // /*
    float max_width = 0.0f;
    shared_ptr<EngineObject> v_item = make_shared<EngineStackedItems>(Vector2{0.0f, 2.0f}, _size, Padding(0), 0);

    std::string base_path = fs::current_path().string() + path;
    for (const auto & entry : fs::recursive_directory_iterator(base_path)){
        std::string text = entry.path().string();
        if (entry.path().extension() == extension) {
            v_item->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, text.substr(text.find(path), text.size()), 18, Padding(5, 0), bind(onclick, text.substr(text.find(path) + 1, text.size()))));
            max_width = v_item->_object_list.back()->_size.x > max_width ? v_item->_object_list.back()->_size.x : max_width;
        }
    }

    addObject(v_item);
    dynamic_pointer_cast<EngineStackedItems>(v_item)->setWidth((int)_size.x);
    // */

    // _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE GAME OBJECT FACADE]
EngineGameObjectFacade::EngineGameObjectFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineStackedItems(offset, size, Padding(0), 1) {
    _game_object = go;

    {
        shared_ptr<EngineObject> new_item = make_shared<EngineSprite>(Vector2{ 0.0f, 0.0f }, Vector2{ _size.x, 20.0f }, 1, Color{ 30, 30, 30, 255 });
        addObject(new_item);

        using placeholders::_1;
        new_item->addObject(make_shared<EngineInput<string>>(Vector2{ 5.0f, 0.0f }, Vector2{ _size.x - 5.0f, 20.0f}, _game_object->name(), bind(&GameObject::setName, _game_object, _1)));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 0, false);
        addObject(new_item);
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Position:", 18, Padding(5, 0)));

        using placeholders::_1;
        new_item->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, _game_object->_offset.x, bind(&GameObject::setXOffset, _game_object, _1)));
        new_item->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, _game_object->_offset.y, bind(&GameObject::setYOffset, _game_object, _1)));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 0, false);
        addObject(new_item);
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Size:", 18, Padding(5, 0)));

        using placeholders::_1;
        new_item->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, _game_object->_size.x, bind(&GameObject::setWidth, _game_object, _1)));
        new_item->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, _game_object->_size.y, bind(&GameObject::setHeight, _game_object, _1)));
    }
    
    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE LABEL FACADE]
EngineLabelFacade::EngineLabelFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineStackedItems(offset, size, Padding(0), 1) {
    _label = go->getBehaviour<Label>();

    using placeholders::_1;

    addTitleObject(_size.x, "Label");
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 0, false);
        addObject(new_item);
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Text:", 18, Padding(5, 0)));

        new_item->addObject(make_shared<EngineInput<string>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3 * 2, 20.0f}, _label->text(), bind(&Label::setText, _label, _1)));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 0, false);
        addObject(new_item);
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Text allign:", 18, Padding(5, 0)));

        shared_ptr<EngineObject> new_object = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3 * 2, 20.0f}, Padding(0, -1, -1, 0), 15, false);
        new_item->addObject(new_object);

        new_object->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{20.0f, 20.0f}, "l", 18, Padding(5, 0), bind(&Label::setAllign, _label, 'l')));
        new_object->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{20.0f, 20.0f}, "c", 18, Padding(5, 0), bind(&Label::setAllign, _label, 'c')));
        new_object->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{20.0f, 20.0f}, "r", 18, Padding(5, 0), bind(&Label::setAllign, _label, 'r')));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 0, false);
        addObject(new_item);

        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Font size:", 18, Padding(5, 0)));
        new_item->addObject(make_shared<EngineInput<int>>(Vector2{0.0f, 0.0f}, Vector2{26.0f, 20.0f}, _label->font_size(), bind(&Label::setFontSize, _label, _1)));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 0, false);
        addObject(new_item);

        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3.0f, 20.0f}, "Color:", 18, Padding(5, 0)));
        new_item->addObject(make_shared<EngineColor>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3.0f * 2.0f - 5.0f, 20.0f}, _label->color(), bind(&Label::setColor, _label, _1)));
    }

    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE SPRITE FACADE]
EngineSpriteFacade::EngineSpriteFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineStackedItems(offset, size, Padding(0), 1) {
    _sprite = go->getBehaviour<Sprite>();

    using placeholders::_1;

    addTitleObject(_size.x, "Sprite");
    addStackedObjects(_size.x, {"Filename:", _sprite->filename() == ""? "No sprite source" : _sprite->filename().c_str(), ""});

    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 0, false);
        addObject(new_item);

        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3.0f, 20.0f}, "Color:", 18, Padding(5, 0)));
        new_item->addObject(make_shared<EngineColor>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3.0f * 2.0f - 5.0f, 20.0f}, _sprite->color(), bind(&Sprite::setcolor, _sprite, _1)));
    }

    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE ANIMATED SPRITE FACADE]
EngineAnimatedSpriteFacade::EngineAnimatedSpriteFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineStackedItems(offset, size, Padding(0), 1) {
    _animated_sprite = go->getBehaviour<AnimatedSprite>();

    addTitleObject(_size.x, "Animated Sprite");

    addStackedObjects(_size.x, {"Filename:", _animated_sprite->filename().c_str(), ""});
    addStackedObjects(_size.x, {"Frame int.:", ftostr(_animated_sprite->time_for_frame(), 3), ""});
    addStackedObjects(_size.x, {"Size:", ftostr(_animated_sprite->size().x, 3), ftostr(_animated_sprite->size().y, 3)});
    addStackedObjects(_size.x, {"Flip:", _animated_sprite->fliph()? "true" : "false", _animated_sprite->flipv()? "true" : "false"});
    
    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE TILE MAP FACADE]
EngineTileMapFacade::EngineTileMapFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineStackedItems(offset, size, Padding(0), 1) {
    _tile_map = go->getBehaviour<TileMap>();

    using placeholders::_1;

    addTitleObject(_size.x, "Tile Map");
    addStackedObjects(_size.x, {"Filename:", _tile_map->filename().c_str(), ""});

    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding(0, -1, -1, 0), 0, false);
        addObject(new_item);

        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Scale:", 18, Padding(5, 0)));
        new_item->addObject(make_shared<EngineInput<int>>(Vector2{0.0f, 0.0f}, Vector2{26.0f, 20.0f}, _tile_map->scale(), bind(&TileMap::setScale, _tile_map, _1)));
    }
    
    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// << FUNCTIONS >>
shared_ptr<EngineObject> CreateList(const Vector2 &offset, const Vector2 &itemDimensions, const vector<string> &items){
    shared_ptr<EngineObject> new_list = make_shared<EngineFitter>(offset, Vector2{itemDimensions.x, 0.0f}, Padding(0, 0, -1, 0), 0);

    for(auto item : items) {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y}, Padding(0, 0, -1, 0), -1);
        new_list->addObject(new_item);

        new_item->addObject(make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y}, 1, Color{30, 30, 30, 255}));
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y}, item, itemDimensions.y - 2.0f, Padding(10, 0)));
    }

    return new_list;
}

shared_ptr<EngineStackedItems> CreateInteractableList(const Vector2 &offset, const Vector2 &itemDimensions, const vector<pair<string, function<void()>>> &items){
    float max_width = 0.0f;
    shared_ptr<EngineStackedItems> new_list = make_shared<EngineStackedItems>(offset, Vector2{itemDimensions.x, 0.0f}, Padding(0), 0);

    for(auto item : items) {
        new_list->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, itemDimensions, item.first, itemDimensions.y - 2.0f, Padding(10, 0), item.second));
        max_width = new_list->_object_list.back()->_size.x > max_width? new_list->_object_list.back()->_size.x : max_width;
    }

    dynamic_pointer_cast<EngineStackedItems>(new_list)->setWidth((int)max_width);
    return new_list;
}

shared_ptr<EngineObject> CreateHierarchyList(const Vector2 &offset, const Vector2 &itemDimensions, const HierarchyActionList &items){
    shared_ptr<EngineObject> new_list = make_shared<EngineStackedItems>(offset, Vector2{itemDimensions.x, 0.0f}, Padding(0), 1);

    for(auto item : items) {
        shared_ptr<EngineObject> new_item = make_shared<EngineObject>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y});
        new_list->addObject(new_item);

        new_item->addObject(make_shared<EngineButton>(Vector2{item.first.first - itemDimensions.y, 0.0f}, Vector2{itemDimensions.y, itemDimensions.y}, item.first.second? "v" : ">", itemDimensions.y - 2.0f, Padding(6, 0), get<0>(item.second.second), false));
        new_item->addObject(make_shared<EngineButton>(Vector2{item.first.first + 1.0f, 0.0f}, Vector2{itemDimensions.x - item.first.first - 1.0f, itemDimensions.y}, item.second.first, itemDimensions.y - 2.0f, Padding(10, 0), get<1>(item.second.second), get<2>(item.second.second), false));
    }

    return new_list;
}

Nary<pair<bool, shared_ptr<GameObject>>> convertToNewTree(Nary<GameObject> a){
    Nary<pair<bool, shared_ptr<GameObject>>> newTree;

    for(int i = 1; i <= a.nBrothers(); i++) {
        Nary<pair<bool, shared_ptr<GameObject>>> newChild(convertToNewTree(a.brother(i).child(0)));
        Nary<pair<bool, shared_ptr<GameObject>>> newBrother(make_shared<pair<bool, shared_ptr<GameObject>>>(pair<bool, shared_ptr<GameObject>>(false, a.brother(i).content())));

        if(!newChild.isEmpty()) newBrother.setChild(newChild);
        newTree.setBrother(newBrother);
    }

    return newTree;
}

HierarchyActionList NaryToActionList(Nary<pair<bool, shared_ptr<GameObject>>> h) {
    HierarchyActionList result;
    float offset = 20.0f;

    for(int i = 1; i <= h.nBrothers(); i++) NaryToActionList_i(h.brother(i), result, offset);

    return result;
}

void NaryToActionList_i(const Nary<pair<bool, shared_ptr<GameObject>>> &a, HierarchyActionList &list, float offset){
    if(!a.isEmpty()) {
        list.push_back({
            {offset, a.content()->first},
            {a.content()->second->name(), {
                bind(&EngineHierarchy::selectItem, EngineHierarchy::getInstance(), a.content()->second),
                bind(&EngineInspector::setGameObject, EngineInspector::getInstance(), a.content()->second),
                bind(&EngineHierarchy::openGameObjectOptions, EngineHierarchy::getInstance(), a.content()->second)
            }}
        });
        offset += 20.0f;

        if(a.content()->first) for(int i = 1; i <= a.nChilds(); i++) NaryToActionList_i(a.child(i), list, offset);
    }
}

void DebugGameObject(shared_ptr<GameObject> go){
    cout << go->name() << " selected\n";
}