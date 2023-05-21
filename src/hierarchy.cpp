#include "raylib.h"
#include "hierarchy.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void Hierarchy::update() {
    Window::update();

    _newGameObject = false;

    if(_options != nullptr) _options->update();
    _objectSelected = false;

    if((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))) {
        if(_options != nullptr) _options = nullptr;
        if(CheckCollisionPointRec(GetMousePosition(), Rectangle{_position.x, _position.y + 30.0f, _size.x, _size.y - 30.0f})){
            shared_ptr<GameObject> possible;

            bool found = false;
            int i = 1;
            Vector2 pos = Vector2{_position.x + 2.0f, _position.y + 30.0f};

            while( i <= _tree.nBrothers() && !found) {
                possible = getGameObjectClicked(_tree.brother(i), GetMousePosition(), pos, 0, found);
                i++;
            }

            if(found) {
                _objectSelected = true;
                _selected = possible;
            }

            if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                using std::placeholders::_1;
                _options = make_shared<EngineOptionList>(Vector2{GetMousePosition().x, GetMousePosition().y}, 150, 18, 
                    vector<pair<string, function<void()>>>{
                        {"Empty GameObject", bind( &Hierarchy::newGameObject, this)},
                        {"Delete GameObject", bind( &Hierarchy::deleteGameObjectSelected, this)}
                    }
                );
            }
        }
    }
}

void Hierarchy::drawTreePostorder(const Vector2 &pos) const {
    Vector2 position = pos;
    for(int i = 1; i <= _tree.nBrothers(); i++) drawTreePostorder_i(_tree.brother(i), position, 0);
}

void Hierarchy::reloadTree(EngineScene _manager) {
    _tree = convertToNewTree(*_manager.getTree());
}

void Hierarchy::draw() {
    Window::draw();
    drawTreePostorder(Vector2{_position.x + 2.0f, _position.y + 30.0f});

    if(_options != nullptr) _options->draw();
}

bool Hierarchy::objectSelected() const{
    return _objectSelected;
}

shared_ptr<GameObject> Hierarchy::getSelected() const{
    return _selected;
}
void Hierarchy::newGameObject() {
    _newObjectCounter ++;
    shared_ptr<NTree<GameObject>> root = _manager.getTree();
    NTree<GameObject> child;
    shared_ptr<GameObject> object = make_shared<GameObject>(GameObject("NewGameObject" + to_string(_newObjectCounter), root));

    object->addComponent(make_shared<TransformComp>(Vector2{0.0f, 0.0f}, Vector2{100.0f, 100.0f}));
    if(!root->isEmpty()) {
        if(root->content()->name() != getSelected()->name()) root->setChild(NTree<GameObject>(child, object));
        else root->find(getSelected()->name()).setChild(NTree<GameObject>(child, object));
    } else root->setBrother(NTree<GameObject>(child, object));

    reloadTree(_manager);
}

void Hierarchy::deleteGameObjectSelected() {
    if(_selected != nullptr) {
        cout << "okay\n";
        _manager.getTree()->find(_selected->name()).remove();
        
        reloadTree(_manager);
    }
}

void Hierarchy::drawTreePostorder_i(const NTree<pair<bool, shared_ptr<GameObject>>> &a, Vector2 &pos, const int &depth) const {
    if(!a.isEmpty()){
        DrawRectangle(pos.x + depth, pos.y, 10, 20, BLUE);
        DrawTextEx(_font, a.content()->second->name().c_str(), Vector2{pos.x + depth + 15, pos.y}, 18, 0, WHITE);
        pos.y += 20;
        if(a.content()->first) {
            if(a.nChilds() > 0) DrawLine(pos.x + depth, pos.y, pos.x + 446, pos.y, BLUE);
            for(int i = 1; i <= a.nChilds(); i++) drawTreePostorder_i(a.child(i), pos, depth + 20);
        }
    }
}

NTree<pair<bool, shared_ptr<GameObject>>> Hierarchy::convertToNewTree(NTree<GameObject> a){
    NTree<pair<bool, shared_ptr<GameObject>>> newTree;

    for(int i = 1; i <= a.nBrothers(); i++) {
        NTree<pair<bool, shared_ptr<GameObject>>> newChild;
        newChild.setBrother(convertToNewTree(a.brother(i).child(0)));
        newTree.setBrother(NTree<pair<bool, shared_ptr<GameObject>>>(newChild, make_shared<pair<bool, shared_ptr<GameObject>>>(pair<bool, shared_ptr<GameObject>>(false, a.brother(i).content()))));
    }

    return newTree;
}

shared_ptr<GameObject> Hierarchy::getGameObjectClicked(const NTree<pair<bool, shared_ptr<GameObject>>> &a, Vector2 mouse, Vector2 &position, const int &depth, bool &found) {
    shared_ptr<GameObject> possible = nullptr;

    if(!a.isEmpty()){
        if(CheckCollisionPointRec(mouse, Rectangle{position.x, position.y, _size.x, 20.0f})) {
            found = true;
            if(mouse.x < position.x + depth + 10) a.content()->first = !a.content()->first;
            possible = a.content()->second;
        }

        position.y += 20;

        if(a.content()->first){
            int i = 1;
            while ( i <= a.nChilds() && !found ) {
                possible = getGameObjectClicked(a.child(i), mouse, position, depth + 20, found);
                i++;
            }
        }
    }

    return possible;
}