#ifndef __HIERARCHY_H__
#define __HIERARCHY_H__

#include "raylib.h"
#include "window.h"
#include "engineScene.h"
#include "engineOptionList.h"
#include <cstdlib>
#include <iostream>

using namespace std;

class Hierarchy : public Window {
    public:
        Hierarchy(Vector2 pos, EngineScene manager) : Window("Hierarchy", pos, Vector2{450, 790}) {
            _newObjectCounter = 0;
            _manager = manager;
            reloadTree(_manager);
            _options = nullptr;
        }

        void update();

        void reloadTree(EngineScene manager);
        void newGameObject();
        void deleteGameObjectSelected();

        void draw();
        void drawTreePostorder(const Vector2 &pos) const;

        bool objectSelected() const;
        shared_ptr<GameObject> getSelected() const;

    private:
        bool _objectSelected, _newGameObject;
        int _newObjectCounter;
        EngineScene _manager;

        NTree<pair<bool, shared_ptr<GameObject>>> _tree;
        shared_ptr<GameObject> _selected;
        shared_ptr<EngineOptionList> _options;

        void drawTreePostorder_i(const NTree<pair<bool, shared_ptr<GameObject>>> &a, Vector2 &pos, const int &depth) const;
        NTree<pair<bool, shared_ptr<GameObject>>> convertToNewTree(NTree<GameObject> a);

        shared_ptr<GameObject> getGameObjectClicked(const NTree<pair<bool, shared_ptr<GameObject>>> &a, Vector2 mouse, Vector2 &position, const int &depth, bool &found);

};

#endif