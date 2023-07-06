#include "platformerEngineUI.h"

using namespace std;
namespace fs = std::filesystem;

// [ENGINE OBJECT]
void EngineObject::addObject(shared_ptr<EngineObject> obj) {
    _object_list.push_back(obj);
}

void EngineObject::addTitleObject(const float &width, const string &title) {
    shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{width, 20.0f}, Padding{0, 0, -1, 0}, -1);
    addObject(new_item);

    new_item->addObject(make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 20.0f}, 1, Color{30, 30, 30, 255}));
    new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 20.0f}, title, 18, Padding{0, 0, 0, 10}));
}

void EngineObject::addStackedObjects(const float &width, const vector<string> &texts) {
    shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{width, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
    addObject(new_item);

    float itemWidth = width / texts.size();

    for(string item : texts)
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{itemWidth, 20.0f}, item, 18, Padding{0, 0, 0, 5}));
}

template<typename N> void EngineObject::addFacadeItem(const float &width, const string &title, const N& item) {

}

void EngineObject::update(const Vector2 &offset) {
    Object::update(offset);
    update();
}

void EngineObject::update() {
    for(auto object : _object_list) object->update(_position);
}

void EngineObject::draw() {
    for(auto object : _object_list) object->draw();
}

int EngineObject::getObjectListSize() const {
    return (int)_object_list.size();
}

// [ENGINE BASE EDITOR]
EngineBaseEditor::EngineBaseEditor(Scene scene) : EngineObject(Vector2{0.0f, 0.0f}, Vector2{0.0f, 0.0f}) {
    _dragging = _moving = false;
    _scene = scene;

    _screen = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()}, Padding{0, 0, 0, 0}, 0);
    _floating = make_shared<EngineObject>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 0.0f});

    shared_ptr<EngineObject> top_layout = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()}, Padding{0, 0, 0, 0}, 0, false);
    _screen->addObject(top_layout);
    {
        shared_ptr<EngineObject> new_object = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{300.0f, 0.0f}, Padding{1, -1, -1, 1}, -1);
        top_layout->addObject(new_object);
        new_object->addObject(EngineHierarchy::getInstance());
        EngineHierarchy::getInstance()->setRoot(_scene.getRoot());
    }
    {
        shared_ptr<EngineObject> new_object = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{300.0f, 0.0f}, Padding{1, 1, -1, -1}, -1);
        top_layout->addObject(new_object);
        new_object->addObject(EngineInspector::getInstance());
    }
    
    shared_ptr<EngineObject> bottom_layout = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()}, Padding{0, 0, 0, 0}, 0, false);
    _screen->addObject(bottom_layout);
    {
        shared_ptr<EngineObject> new_object = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{800.0f, 200.0f}, Padding{-1, -1, 1, 1}, -1);
        bottom_layout->addObject(new_object);
        new_object->addObject(EngineAnimationEditor::getInstance());
        EngineAnimationEditor::getInstance()->setBaseObject(_floating);
    }

    _floating->addObject(EngineTileSetEditor::getInstance());
    EngineTileSetEditor::getInstance()->setBaseObject(_floating);
}

void EngineBaseEditor::update() {
    if(!_moving) _prev_position = _position;
    if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
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

    _scene.update(_position);
    _screen->update();
    _floating->update();

    EngineObject::update();
}

void EngineBaseEditor::draw() {
    Vector2 origin{float(int(_position.x) % 50) - 50, float(int(_position.y) % 50) - 50};

    for(int i = origin.x; i <= GetScreenWidth() + 100; i += 50) 
        DrawLine(i, origin.y, i, origin.y + GetScreenHeight() + 100, i == _position.x? GREEN : Color{255, 255, 255, 100});
    
    for(int i = origin.y; i <= GetScreenWidth() + 100; i += 50) 
        DrawLine(origin.x, i, origin.x + GetScreenWidth() + 100, i, i == _position.y? RED : Color{255, 255, 255, 100});

    _scene.draw();
    _screen->draw();
    _floating->draw();

    EngineObject::draw();
}

// [ENGINE COLOR WHEEL]
void EngineColorWheel::update() {
    _cancel_hover = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 160, _position.y + 150, 80, 20});
    _confirm_hover = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 255, _position.y + 150, 80, 20});

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if(_confirm_hover) {
            _on_confirm(_current);
            _close = true;
        }
    }
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 35, _size.x - 65.0f, 20}))
            _current.r = (GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2);
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 57, _size.x - 65.0f, 20}))
            _current.g = (GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2);
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 79, _size.x - 65.0f, 20}))
            _current.b = (GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2);
        else if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x + 20, _position.y + 101, _size.x - 65.0f, 20}))
            _current.a = (GetMousePosition().x - (_position.x + 20)) * 255.0f / (_size.x - 65.0f - 2);
    }

    EngineObject::update();
}

void EngineColorWheel::draw() {
    float block_size = _size.x - 65.0f;

    DrawTextEx(_font, "R", Vector2{_position.x + 5, _position.y + 35}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 35, block_size, 20}, 2, BLACK);
    DrawRectangle(_position.x + 20 + _current.r / 255.0f * (block_size - 2), _position.y + 35, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.r).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 35}, 18, 0, BLACK);

    DrawTextEx(_font, "G", Vector2{_position.x + 5, _position.y + 57}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 57, block_size, 20}, 2, BLACK);
    DrawRectangle(_position.x + 20 + _current.g / 255.0f * (block_size - 2), _position.y + 57, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.g).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 57}, 18, 0, BLACK);

    DrawTextEx(_font, "B", Vector2{_position.x + 5, _position.y + 79}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 79, block_size, 20}, 2, BLACK);
    DrawRectangle(_position.x + 20 + _current.b / 255.0f * (block_size - 2), _position.y + 79, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.b).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 79}, 18, 0, BLACK);

    DrawTextEx(_font, "A", Vector2{_position.x + 5, _position.y + 101}, 18, 0, BLACK);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 101, block_size, 20}, 2, BLACK);
    DrawRectangle(_position.x + 20 + _current.a / 255.0f * (block_size - 2), _position.y + 101, 3, 20, RED);
    DrawTextEx(_font, to_string(_current.a).c_str(), Vector2{_position.x + 20 + block_size + 10, _position.y + 101}, 18, 0, BLACK);

    DrawRectangleRec(Rectangle{_position.x + 20, _position.y + 123, block_size, 20}, _current);
    DrawRectangleLinesEx(Rectangle{_position.x + 20, _position.y + 123, block_size, 20}, 2, BLACK);

    DrawRectangleRec(Rectangle{_position.x + 160, _position.y + 150, 80, 20}, _cancel_hover? BLUE : GRAY);
    DrawTextEx(_font, "CANCEL", Vector2{_position.x + 175, _position.y + 150}, 18, 0, BLACK);
    DrawRectangleRec(Rectangle{_position.x + 255, _position.y + 150, 80, 20}, _confirm_hover? BLUE : GRAY);
    DrawTextEx(_font, "CONFIRM", Vector2{_position.x + 265, _position.y + 150}, 18, 0, BLACK);

    EngineObject::draw();
}

// [ENGINE FITTER]
void EngineFitter::addObject(shared_ptr<EngineObject> obj) {
    EngineObject::addObject(obj);

    int n = _object_list.size(), i = 0;
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

// void EngineFitter::draw() {
//     DrawRectangleLines(_position.x, _position.y, _size.x, _size.y, BLUE);

//     EngineObject::draw();
// }

// [ENGINE LABEL]
void EngineLabel::draw() {
    Rectangle rec{_position.x + _padding.left, _position.y + _padding.top, _size.x - _padding.left - _padding.right, _size.y - _padding.top - _padding.bottom};
    DrawTextRecEx(_font, _text.c_str(), rec, _font_size, 0, true, WHITE, 0, 0, WHITE, BLUE);
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

void EngineSprite::draw() {
    if(_texture != nullptr) DrawTextureEx(*_texture, _position, 0.0f, _scale, _color);
    else DrawRectangle(_position.x, _position.y, _size.x * _scale, _size.y * _scale, _color);

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
        int i = value % (int)(_actual->_data->_nTiles.x), j = floor(value / (int)(_actual->_data->_nTiles.x));

        Rectangle src = Rectangle{i * _actual->_data->_tileSize.x, j * _actual->_data->_tileSize.y, _actual->_data->_tileSize.x, _actual->_data->_tileSize.y};
        Rectangle dst = Rectangle{_position.x, _position.y, _size.x, _size.y};

        DrawTexturePro(*_actual->_data->_texture, src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    }else DrawRectangle(_position.x, _position.y, _size.x, _size.y, Color{90, 90, 90, 255});
}

// [ENGINE BUTTON]
void EngineButton::update() {
    Rectangle rec{_position.x, _position.y, _size.x, _size.y - 1.0f};
    _hover = CheckCollisionPointRec(GetMousePosition(), rec);

    if(_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) _on_click();
    if(_right_click && _hover && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) _on_right_click();

    EngineObject::update();
}

void EngineButton::draw() {
    Rectangle rec{_position.x + _padding.left, _position.y + _padding.top, _size.x - _padding.left - _padding.right, _size.y - _padding.top - _padding.bottom};
    DrawRectangle(_position.x, _position.y, _size.x, _size.y, _hover? BLUE : Color{30, 30, 30, 255});

    DrawTextRecEx(_font, _text.c_str(), rec, _font_size, 0, true, WHITE, 0, 0, WHITE, WHITE);

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
        addObject(make_shared<EngineColorWheel>(Vector2{-450.0f, 0.0f}, _color, bind(&EngineColor::setColor, this, _1)));
    }

    EngineObject::update();
}

void EngineColor::draw() {
    DrawRectangleRec(Rectangle{_position.x, _position.y, _size.x, _size.y}, Color{_color.r, _color.g, _color.b, 255});
    DrawRectangleRec(Rectangle{_position.x, _position.y + _size.y - 3.0f, _size.x, 3.0f}, RED);
    DrawRectangleRec(Rectangle{_position.x, _position.y + _size.y - 3.0f, _color.a / 255 * _size.x, 3.0f}, WHITE);

    DrawRectangleLines(_position.x, _position.y, _size.x + 1, _size.y, BLACK);

    EngineObject::draw();
}

// [ENGINE INPUT]
template<class T>
void EngineInput<T>::selectInput() {
    _hover = CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x, _position.y, _size.x, _size.y});
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        _selected   = _hover;
        _cursor_pos = _text.size();
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

        if(IsKeyPressed(KEY_BACKSPACE)) _text = _text.substr(0, _cursor_pos - 1) + ((int)_text.size() > _cursor_pos? _text.substr(_cursor_pos, _text.size()) : "");
        else if(IsKeyPressed(KEY_DELETE)) _text = _text.substr(0, _cursor_pos) + _text.substr(_cursor_pos + 1, _text.size());
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

        if(IsKeyPressed(KEY_BACKSPACE)) _text = _text.substr(0, _cursor_pos - 1) + ((int)_text.size() > _cursor_pos? _text.substr(_cursor_pos, _text.size()) : "");
        else if(IsKeyPressed(KEY_DELETE)) _text = _text.substr(0, _cursor_pos) + _text.substr(_cursor_pos + 1, _text.size());
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
            if(_decimal && _text[_cursor_pos - 1] == '.') _decimal = false;
            _text = _text.substr(0, _cursor_pos - 1) + ((int)_text.size() > _cursor_pos? _text.substr(_cursor_pos, _text.size()) : "");
        }else if(IsKeyPressed(KEY_DELETE)) {
            if(_decimal && _text[_cursor_pos] == '.') _decimal = false;
            _text = _text.substr(0, _cursor_pos) + _text.substr(_cursor_pos + 1, _text.size());
        }
    }

    updateCursor();
}

template<>
EngineInput<string>::EngineInput(Vector2 offset, Vector2 size, string item, function<void(string)> set_text) : EngineObject(offset, size), _item(item) {
    _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
    _selected = false;
    _text = _item;
    _initial_pos = 0;
    _set_text = set_text;
}

template<>
EngineInput<int>::EngineInput(Vector2 offset, Vector2 size, int item, function<void(int)> set_text) : EngineObject(offset, size), _item(item) {
    _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
    _selected = false;
    _text = to_string(_item);
    _initial_pos = 0;
    _set_text = set_text;
}

template<>
EngineInput<float>::EngineInput(Vector2 offset, Vector2 size, float item, function<void(float)> set_text) : EngineObject(offset, size), _item(item) {
    _font = *Resources::getInstance()->font("resources/monogram.ttf", true);
    _selected = false;
    _text = to_string(_item);
    _initial_pos = 0;
    _set_text = set_text;
}

template<>
void EngineInput<string>::update(){
    updateText();

    if(_selected && IsKeyPressed(KEY_ENTER)) {
        _set_text(_text);
        _selected = false;
        _cursor_pos = -1;
    }

    EngineObject::update();
}

template<>
void EngineInput<int>::update(){
    updateText();

    if(_selected && IsKeyPressed(KEY_ENTER)) {
        _set_text(stoi(_text));
        _selected = false;
        _cursor_pos = -1;
    }

    EngineObject::update();
}

template<>
void EngineInput<float>::update(){
    updateText();

    if(_selected && IsKeyPressed(KEY_ENTER)) {
        _set_text(stof(_text));
        _selected = false;
        _cursor_pos = -1;
    }

    EngineObject::update();
}

template<class T>
void EngineInput<T>::draw(){
    Rectangle rec{_position.x + 1, _position.y, _size.x, _size.y};

    int text_size = _size.x / 8;

    if(_selected) _initial_pos = _cursor_pos - _initial_pos > text_size? _initial_pos + 1 : _cursor_pos - _initial_pos < 0? _initial_pos - 1 : _initial_pos;

    string n_text = _text.substr(_initial_pos, _text.size() - _initial_pos);
    DrawTextRecEx(_font, n_text.c_str(), rec, 18, 0, false, WHITE, 0, 0, WHITE, BLUE);

    if(_selected) DrawRectangle((_cursor_pos - _initial_pos) * 8 + _position.x + 1, _position.y + 17, 8, 2, BLUE);
    DrawRectangleLines(_position.x, _position.y, _size.x, _size.y, _hover || _selected? BLUE : Color{60, 60, 60, 255});

    EngineObject::draw();
}

// [ENGINE OPTION LIST]
EngineOptionList::EngineOptionList(const Vector2 &pos, const Vector2 &size, const Vector2 &itemDimensions, const vector<pair<string, function<void()>>> &items) : EngineFitter(pos, size, Padding{0, -1, -1, -1}, 0, false){
    addObject(CreateInteractableList(_position, itemDimensions, items));
}

// [ENGINE HIERARCHY]
shared_ptr<EngineHierarchy> EngineHierarchy::_instance = nullptr;
shared_ptr<EngineHierarchy> EngineHierarchy::getInstance() {
    if(_instance == nullptr) _instance = make_shared<EngineHierarchy>(Vector2{0.0f, 0.0f}, Vector2{350.0f, 0.0f});
    return _instance;
}

EngineHierarchy::EngineHierarchy(Vector2 offset, Vector2 size) : EngineFitter(offset, size, Padding{0, -1, -1, 0}, 1) {
    addTitleObject(size.x, "Hierarchy");
}

EngineHierarchy::EngineHierarchy(Vector2 offset, Vector2 size, shared_ptr<Nary<GameObject>> root) : EngineFitter(offset, size, Padding{0, -1, -1, 0}, 1) {
    setRoot(root);
}

void EngineHierarchy::setRoot(shared_ptr<Nary<GameObject>> root){
    _object_list.clear();
    _root = root;
    addTitleObject(_size.x, "Hierarchy");

    _hierarchyTree = convertToNewTree(*_root);

    HierarchyActionList action_list = NaryToActionList(_hierarchyTree);
    addObject(CreateHierarchyList(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, action_list));
}

void EngineHierarchy::selectItem(shared_ptr<GameObject> go){
    _object_list.clear();
    addTitleObject(_size.x, "Hierarchy");

    for(int i = 1; i <= _hierarchyTree.nBrothers(); i++) selectItem_i(_hierarchyTree.brother(i), go);

    HierarchyActionList action_list = NaryToActionList(_hierarchyTree);
    addObject(CreateHierarchyList(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, action_list));
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
        // {"New Label", bind(&EngineHierarchy::addBehaviour, this, make_shared<Label>("new label", 18))},
        // {"New Sprite", bind(&EngineHierarchy::addBehaviour, this, make_shared<Sprite>(WHITE))},
        // {"New Animated Sprite", bind(&EngineHierarchy::addBehaviour, this, make_shared<AnimatedSprite>())},
        // {"New Tile Map", bind(&EngineHierarchy::addBehaviour, this, make_shared<TileMap>())}
    };

    addObject(make_shared<EngineOptionList>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Vector2{250.0f, 20.0f}, action_list));
}

void EngineHierarchy::addGameObject(shared_ptr<GameObject> go) {
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

EngineInspector::EngineInspector(Vector2 offset, Vector2 size) : EngineFitter(offset, size, Padding{0, -1, -1, 0}, 5) {
    addTitleObject(_size.x, "Inspector");
}

EngineInspector::EngineInspector(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineFitter(offset, size, Padding{0, -1, -1, 0}, 5) {
    setGameObject(go);
}

void EngineInspector::setGameObject(shared_ptr<GameObject> go) {
    _object_list.clear();
    _game_object = go;

    addTitleObject(_size.x, "Inspector");
    if(_game_object != nullptr) {
        addObject(make_shared<EngineGameObjectFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));

        for(auto item : _game_object->getBehaviours()){
            if(dynamic_pointer_cast<Label>(item) != nullptr) addObject(make_shared<EngineLabelFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));
            else if(dynamic_pointer_cast<Sprite>(item) != nullptr) addObject(make_shared<EngineSpriteFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));
            else if(dynamic_pointer_cast<AnimatedSprite>(item) != nullptr) addObject(make_shared<EngineAnimatedSpriteFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));
            else if(dynamic_pointer_cast<TileMap>(item) != nullptr) addObject(make_shared<EngineTileMapFacade>(Vector2{0.0f, 0.0f}, _size, _game_object));
        }

        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, -1}, 0, false);
        addObject(new_item);

        shared_ptr<EngineObject> new_button = make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 4.0f * 3.0f, 20.0f}, "Add Behaviour", 18, Padding{0, -1, 0, 5}, bind(&EngineInspector::openNewBehaviorOptions, this));
        new_item->addObject(new_button);
    }
}

void EngineInspector::openNewBehaviorOptions(){
    vector<pair<string, function<void()>>> action_list{
        {"New Label", bind(&EngineInspector::addBehaviour<Label>, this, make_shared<Label>("new label", 18))},
        {"New Sprite", bind(&EngineInspector::addBehaviour<Sprite>, this, make_shared<Sprite>(WHITE))},
        {"New Animated Sprite", bind(&EngineInspector::addBehaviour<AnimatedSprite>, this, make_shared<AnimatedSprite>())},
        {"New Tile Map", bind(&EngineInspector::addBehaviour<TileMap>, this, make_shared<TileMap>())}
    };

    addObject(make_shared<EngineOptionList>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Vector2{250.0f, 20.0f}, action_list));
}

// [ENGINE ANIMATION EDITOR]
shared_ptr<EngineAnimationEditor>EngineAnimationEditor::_instance = nullptr;
shared_ptr<EngineAnimationEditor>EngineAnimationEditor::getInstance() {
    if(_instance == nullptr) _instance = make_shared<EngineAnimationEditor>(Vector2{0.0f, 0.0f}, Vector2{800.0f, 0.0f});
    return _instance;
}

EngineAnimationEditor::EngineAnimationEditor(Vector2 offset, Vector2 size) : EngineFitter(offset, size, Padding{0, -1, -1, 0}, 1) {
    addTitleObject(_size.x, "Animation Editor");

    _options = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 1, false);
    addObject(_options);

    _sequences = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 100.0f}, Padding{0, -1, -1, 0}, 0, false);
    addObject(_sequences);
    
    shared_ptr<EngineObject> background_1 = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{200.0f, 100.0f}, Padding{0, -1, -1, 0}, -1);
    _selector = make_shared<EngineAnimationSelector>(Vector2{0.0f, 0.0f}, Vector2{199.f, 100.0f});
    background_1->addObject(make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, Vector2{199.f, 100.0f}, 1, Color{60, 60, 60, 255}));
    background_1->addObject(_selector);

    shared_ptr<EngineObject> background_2 = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x - 200.0f, 100.0f}, Padding{0, -1, -1, 0}, -1);
    _sequence = make_shared<EngineAnimationSequence>(Vector2{0.0f, 0.0f}, Vector2{_size.x - 201.f, 100.0f});
    background_2->addObject(make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, Vector2{_size.x - 201.f, 100.0f}, 1, Color{60, 60, 60, 255}));
    background_2->addObject(_sequence);

    _sequences->addObject(background_1);
    _sequences->addObject(background_2);
    
    _options->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{120.f, 20.0f}, "New Animation", 18, Padding{0, 0, 0, 5}, bind(&EngineAnimationEditor::addNewAnimation, this)));
    _options->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{ 90.f, 20.0f}, "New Point", 18, Padding{0, 0, 0, 5}, bind(&EngineAnimationSequence::addNewPoint, dynamic_pointer_cast<EngineAnimationSequence>(_sequence))));
    _options->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{125.f, 20.0f}, "Select TileSet", 18, Padding{0, 0, 0, 5}, bind(&EngineAnimationEditor::selectTileSet, this)));

    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
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
    using placeholders::_1;

    _obj->addObject(make_shared<EngineFileSelector>(
        Vector2{0.0f, 500.0f}, Vector2{_size.x, 400.0f}, "\\resources", ".tsd", 
        bind(&EngineAnimationEditor::setTexture, this, _1)
    ));
}

void EngineAnimationEditor::setTexture(string path) {
    dynamic_pointer_cast<EngineAnimationSequence>(_sequence)->setTexture(path);
    
    int i = 0;
    bool found = false;
    while(i < (int)_obj->_object_list.size() && !found) {
        if(auto j = dynamic_pointer_cast<EngineFileSelector>(_obj->_object_list[i])) found = true;
        else i++;
    }

    if(found) _obj->_object_list.erase(_obj->_object_list.begin()+i);
}

void EngineAnimationEditor::setBaseObject(shared_ptr<EngineObject> obj) {
    _obj = obj;
}

// [ENGINE ANIMATION SELECTOR]
EngineAnimationSelector::EngineAnimationSelector(Vector2 offset, Vector2 size) : EngineFitter(offset, size, Padding{0, -1, -1, 0}, 1) {
    
}

void EngineAnimationSelector::addAnimation(shared_ptr<EngineAnimation> anim, function<void()> select){
    _animation_list.push_back(anim);

    addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, anim->_name, 18, Padding{0, 0, 0, 5}, select));
}

// [ENGINE ANIMATION SEQUENCE]
EngineAnimationSequence::EngineAnimationSequence(Vector2 offset, Vector2 size) : EngineFitter(offset, size, Padding{0, 0, -1, 0}, 1) {
    _actual = nullptr;

    _timers = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 1, false);
    addObject(_timers);

    _values = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 60.0f}, Padding{0, -1, -1, 0}, 1, false);
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

    _timers = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 1, false);
    addObject(_timers);

    _values = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 60.0f}, Padding{0, -1, -1, 0}, 1, false);
    addObject(_values);

    for(auto item : _actual->_sequence) addPoint(item.first, item.second);
}

void EngineAnimationSequence::setTexture(string path) {
    _actual->setData(Resources::getInstance()->tileset(path, true));
}

// [ENGINE TILE SET EDITOR]
shared_ptr<EngineTileSetEditor>EngineTileSetEditor::_instance = nullptr;
shared_ptr<EngineTileSetEditor>EngineTileSetEditor::getInstance() {
    if(_instance == nullptr) _instance = make_shared<EngineTileSetEditor>(Vector2{(float)GetScreenWidth() - 500.0f, 500.0f}, Vector2{500.0f, 600.0f});
    return _instance;
}

EngineTileSetEditor::EngineTileSetEditor(Vector2 offset, Vector2 size) : EngineFitter(offset, size, Padding{0, -1, -1, 0}, 1) {
    addTitleObject(_size.x, "TileSet Editor");

    shared_ptr<EngineObject> options = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 1, false);
    addObject(options);

    options->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{130.f, 20.0f}, "Select Texture", 18, Padding{0, 0, 0, 5}, bind(&EngineTileSetEditor::selectTexture, this)));
    options->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{50.f, 20.0f}, "Save", 18, Padding{0, 0, 0, 5}, bind(&EngineTileSetEditor::saveData, this)));
    

    using placeholders::_1;
    options->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{55.0f, 20.0f}, " Rows:", 18, Padding{0, 0, 0, 5}));
    options->addObject(make_shared<EngineInput<int>>(Vector2{0.0f, 0.0f}, Vector2{20.0f, 20.0f}, 1, bind(&EngineTileSetEditor::setRows, this, _1)));
    options->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{80.0f, 20.0f}, " Columns:", 18, Padding{0, 0, 0, 5}));
    options->addObject(make_shared<EngineInput<int>>(Vector2{0.0f, 0.0f}, Vector2{20.0f, 20.0f}, 1, bind(&EngineTileSetEditor::setColumns, this, _1)));
    
    _sprite = make_shared<EngineTileSetView>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 400.0f});
    addObject(_sprite);

    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

void EngineTileSetEditor::selectTexture() {
    using placeholders::_1;

    _obj->addObject(make_shared<EngineFileSelector>(
        Vector2{0.0f, 500.0f}, Vector2{_size.x, 400.0f}, "\\resources", ".png", 
        bind(&EngineTileSetEditor::setTexture, this, _1)
    ));
}

void EngineTileSetEditor::setTexture(string path) {
    dynamic_pointer_cast<EngineTileSetView>(_sprite)->setTexture(path);
    _data = make_shared<TileSetData>();
    _data->_texture = Resources::getInstance()->texture(path, true);

    int i = 0;
    bool found = false;
    while(i < (int)_obj->_object_list.size() && !found) {
        if(auto j = dynamic_pointer_cast<EngineFileSelector>(_obj->_object_list[i])) found = true;
        else i++;
    }

    if(found) _obj->_object_list.erase(_obj->_object_list.begin()+i);
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
EngineTileSetView::EngineTileSetView(Vector2 offset, Vector2 size) : EngineSprite(offset, size, 1, Color{120, 120, 120, 255}) {
    _nTiles = Vector2{1.0f, 1.0f};
}

void EngineTileSetView::setTexture(string path) {
    shared_ptr<Texture2D> text = Resources::getInstance()->texture(path, true);

    EngineSprite::setTexture(text);
    EngineSprite::setScale(_size.x / text->width);
}

void EngineTileSetView::draw() {
    if(EngineSprite::texture() != nullptr) {
        for(int i = 1; i < _nTiles.x; i++) 
            DrawLine(_position.x + _size.x / _nTiles.x * i, _position.y, _position.x + _size.x / _nTiles.x * i, _position.y + EngineSprite::texture()->height * EngineSprite::scale(), BLUE);
        for(int i = 1; i < _nTiles.y; i++) 
            DrawLine(_position.x, _position.y + EngineSprite::texture()->height * EngineSprite::scale() / _nTiles.y * i, _position.x + _size.x, _position.y + EngineSprite::texture()->height * EngineSprite::scale() / _nTiles.y * i, BLUE);
        DrawRectangleLines(_position.x, _position.y, _size.x, EngineSprite::texture()->height * EngineSprite::scale(), BLUE);
    }
    
    EngineSprite::draw();
}

void EngineTileSetView::setRows(int i){
    _nTiles.y = (float)i;
}

void EngineTileSetView::setColumns(int i){
    _nTiles.x = (float)i;
}

// [ENGINE FILE SELECTOR]
EngineFileSelector::EngineFileSelector(Vector2 offset, Vector2 size, string path, string extension, function<void(string)> onclick) : EngineFitter(offset, size, Padding{0, -1, -1, 0}, 1) {
    addTitleObject(_size.x, "File Explorer");

    shared_ptr<EngineObject> h_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, _size.y - 20.0f}, Padding{0, -1, -1, 0}, 0, false);
    h_item->addObject(make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, Vector2{20.0f, _size.y - 20.0f}, 1, Color{90, 90, 90, 255}));
    shared_ptr<EngineObject> v_item = make_shared<EngineFitter>(offset, Vector2{_size.x - 20.0f, _size.y - 20.0f}, Padding{0, -1, -1, 0}, 1);

    std::string base_path = fs::current_path().string() + path;
    for (const auto & entry : fs::recursive_directory_iterator(base_path)){
        std::string text = entry.path().string();
        if(entry.path().extension() == extension) 
            v_item->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, text.substr(text.find(path), text.size()), 18, Padding{0, -1, -1, 5}, bind(onclick, text.substr(text.find(path) + 1, text.size()))));
    }

    addObject(h_item);
    addObject(v_item);

    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE GAME OBJECT FACADE]
EngineGameObjectFacade::EngineGameObjectFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineFitter(offset, size, Padding{0, 0, -1, 0}, 1) {
    _game_object = go;

    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, 0, -1, 0}, -1);
        addObject(new_item);

        new_item->addObject(make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 20.0f}, 1, Color{30, 30, 30, 255}));

        using placeholders::_1;
        new_item->addObject(make_shared<EngineInput<string>>(Vector2{0.0f, 0.0f}, Vector2{0.0f, 20.0f}, _game_object->name(), bind(&GameObject::setName, _game_object, _1)));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
        addObject(new_item);
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Position:", 18, Padding{0, 0, 0, 5}));

        using placeholders::_1;
        new_item->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, _game_object->_offset.x, bind(&GameObject::setXOffset, _game_object, _1)));
        new_item->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, _game_object->_offset.y, bind(&GameObject::setYOffset, _game_object, _1)));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
        addObject(new_item);
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Size:", 18, Padding{0, 0, 0, 5}));

        using placeholders::_1;
        new_item->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, _game_object->_size.x, bind(&GameObject::setWidth, _game_object, _1)));
        new_item->addObject(make_shared<EngineInput<float>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, _game_object->_size.y, bind(&GameObject::setHeight, _game_object, _1)));
    }
    
    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE LABEL FACADE]
EngineLabelFacade::EngineLabelFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineFitter(offset, size, Padding{0, 0, -1, 0}, 1) {
    _label = go->getBehaviour<Label>();

    using placeholders::_1;

    addTitleObject(_size.x, "Label");
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
        addObject(new_item);
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Text:", 18, Padding{0, 0, 0, 5}));

        new_item->addObject(make_shared<EngineInput<string>>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3 * 2, 20.0f}, _label->text(), bind(&Label::setText, _label, _1)));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
        addObject(new_item);
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Text allign:", 18, Padding{0, 0, 0, 5}));

        shared_ptr<EngineObject> new_object = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3 * 2, 20.0f}, Padding{0, -1, -1, 0}, 15, false);
        new_item->addObject(new_object);

        new_object->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{20.0f, 20.0f}, "l", 18, Padding{0, -1, -1, 5}, bind(&Label::setAllign, _label, 'l')));
        new_object->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{20.0f, 20.0f}, "c", 18, Padding{0, -1, -1, 5}, bind(&Label::setAllign, _label, 'c')));
        new_object->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{20.0f, 20.0f}, "r", 18, Padding{0, -1, -1, 5}, bind(&Label::setAllign, _label, 'r')));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
        addObject(new_item);

        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Font size:", 18, Padding{0, 0, 0, 5}));
        new_item->addObject(make_shared<EngineInput<int>>(Vector2{0.0f, 0.0f}, Vector2{26.0f, 20.0f}, _label->font_size(), bind(&Label::setFontSize, _label, _1)));
    }
    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
        addObject(new_item);

        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3.0f, 20.0f}, "Color:", 18, Padding{0, 0, 0, 5}));
        new_item->addObject(make_shared<EngineColor>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3.0f * 2.0f - 5.0f, 20.0f}, _label->color(), bind(&Label::setColor, _label, _1)));
    }

    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE SPRITE FACADE]
EngineSpriteFacade::EngineSpriteFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineFitter(offset, size, Padding{0, 0, -1, 0}, 1) {
    _sprite = go->getBehaviour<Sprite>();

    using placeholders::_1;

    addTitleObject(_size.x, "Sprite");
    addStackedObjects(_size.x, {"Filename:", _sprite->filename() == ""? "No sprite source" : _sprite->filename().c_str(), ""});

    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
        addObject(new_item);

        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3.0f, 20.0f}, "Color:", 18, Padding{0, 0, 0, 5}));
        new_item->addObject(make_shared<EngineColor>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3.0f * 2.0f - 5.0f, 20.0f}, _sprite->color(), bind(&Sprite::setcolor, _sprite, _1)));
    }

    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE ANIMATED SPRITE FACADE]
EngineAnimatedSpriteFacade::EngineAnimatedSpriteFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineFitter(offset, size, Padding{0, 0, -1, 0}, 1) {
    _animated_sprite = go->getBehaviour<AnimatedSprite>();

    addTitleObject(_size.x, "Animated Sprite");

    addStackedObjects(_size.x, {"Filename:", _animated_sprite->filename().c_str(), ""});
    addStackedObjects(_size.x, {"Frame int.:", ftostr(_animated_sprite->time_for_frame(), 3), ""});
    addStackedObjects(_size.x, {"Size:", ftostr(_animated_sprite->size().x, 3), ftostr(_animated_sprite->size().y, 3)});
    addStackedObjects(_size.x, {"Flip:", _animated_sprite->fliph()? "true" : "false", _animated_sprite->flipv()? "true" : "false"});
    
    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// [ENGINE TILE MAP FACADE]
EngineTileMapFacade::EngineTileMapFacade(Vector2 offset, Vector2 size, shared_ptr<GameObject> go) : EngineFitter(offset, size, Padding{0, 0, -1, 0}, 1) {
    _tile_map = go->getBehaviour<TileMap>();

    using placeholders::_1;

    addTitleObject(_size.x, "Tile Map");
    addStackedObjects(_size.x, {"Filename:", _tile_map->filename().c_str(), ""});

    {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{_size.x, 20.0f}, Padding{0, -1, -1, 0}, 0, false);
        addObject(new_item);

        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{_size.x / 3, 20.0f}, "Scale:", 18, Padding{0, 0, 0, 5}));
        new_item->addObject(make_shared<EngineInput<int>>(Vector2{0.0f, 0.0f}, Vector2{26.0f, 20.0f}, _tile_map->scale(), bind(&TileMap::setScale, _tile_map, _1)));
    }
    
    _object_list.insert(_object_list.begin(), make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, _size, 1, Color{90, 90, 90, 255}));
}

// << FUNCTIONS >>
shared_ptr<EngineObject> CreateList(const Vector2 &offset, const Vector2 &itemDimensions, const vector<string> &items){
    shared_ptr<EngineObject> new_list = make_shared<EngineFitter>(offset, Vector2{itemDimensions.x, 0.0f}, Padding{0, 0, -1, 0}, 0);

    for(auto item : items) {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y}, Padding{0, 0, -1, 0}, -1);
        new_list->addObject(new_item);

        new_item->addObject(make_shared<EngineSprite>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y}, 1, Color{30, 30, 30, 255}));
        new_item->addObject(make_shared<EngineLabel>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y}, item, itemDimensions.y - 2.0f, Padding{0, 10, 0, 10}));
    }

    return new_list;
}

shared_ptr<EngineObject> CreateInteractableList(const Vector2 &offset, const Vector2 &itemDimensions, const vector<pair<string, function<void()>>> &items){
    shared_ptr<EngineObject> new_list = make_shared<EngineFitter>(offset, Vector2{itemDimensions.x, 0.0f}, Padding{0, -1, -1, 0}, 0);

    for(auto item : items) {
        shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, itemDimensions, Padding{0, 0, -1, 0}, -1);
        new_list->addObject(new_item);

        new_item->addObject(make_shared<EngineButton>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y}, item.first, itemDimensions.y - 2.0f, Padding{0, 0, 0, 10}, item.second));
    }

    return new_list;
}

shared_ptr<EngineObject> CreateHierarchyList(const Vector2 &offset, const Vector2 &itemDimensions, const HierarchyActionList &items){
    shared_ptr<EngineObject> new_list = make_shared<EngineFitter>(offset, Vector2{itemDimensions.x, 0.0f}, Padding{0, 0, -1, 0}, 1);

    for(auto item : items) {
        // shared_ptr<EngineObject> new_item = make_shared<EngineFitter>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y}, Padding{0, 0, -1, -1}, -1);
        shared_ptr<EngineObject> new_item = make_shared<EngineObject>(Vector2{0.0f, 0.0f}, Vector2{0.0f, itemDimensions.y});
        new_list->addObject(new_item);

        // cout << item.first << " " << item.second.first << "\n";
        new_item->addObject(make_shared<EngineButton>(Vector2{item.first.first - itemDimensions.y, 0.0f}, Vector2{itemDimensions.y, itemDimensions.y}, item.first.second? "|" : "-", itemDimensions.y - 2.0f, Padding{0, 5, 0, 5}, get<0>(item.second.second)));
        new_item->addObject(make_shared<EngineButton>(Vector2{item.first.first + 1.0f, 0.0f}, Vector2{itemDimensions.x - item.first.first - 1.0f, itemDimensions.y}, item.second.first, itemDimensions.y - 2.0f, Padding{0, 10, 0, 10}, get<1>(item.second.second), get<2>(item.second.second)));
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