#ifndef __HIERARCHY_H__
#define __HIERARCHY_H__

#include "raylib.h"
#include "window.h"
#include "engineScene.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class Hierarchy : public Window {
    public:
        Hierarchy(Vector2 pos, EngineScene manager) : Window("Hierarchy", pos, Vector2{450, 800}){
            // _tree = manager.getTree();
            _tree = convertToNewTree(manager.getTree());
        }

        void update();

        void drawTreePostorder(const Vector2 &pos) const;
        void draw();

        bool objectSelected() const;
        shared_ptr<GameObject> getSelected() const;

    private:
        NTree<pair<bool, shared_ptr<GameObject>>> _tree;
        bool _objectSelected;
        shared_ptr<GameObject> _selected;

        void drawTreePostorder_i(const NTree<pair<bool, shared_ptr<GameObject>>> &a, Vector2 &pos, const int &depth) const;
        NTree<pair<bool, shared_ptr<GameObject>>> convertToNewTree(NTree<GameObject> a);

        shared_ptr<GameObject> getGameObjectClicked(const NTree<pair<bool, shared_ptr<GameObject>>> &a, Vector2 mouse, Vector2 &position, const int &depth, bool &found);

};

#endif