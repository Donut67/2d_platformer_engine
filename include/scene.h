#ifndef __SCENE_H__
#define __SCENE_H__

#include "raylib.h"
#include "nTree.h"
#include "gameObject.h"
// #include "bulkfile.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

using namespace std;

class GameObject;

class Scene : public enable_shared_from_this<Scene>{
    public:
        // [[ CONSTRUCTORS ]]
        Scene();
        // pre:  true
        // post: default constructor
        Scene(vector<string> fileNames, const int &act);
        // pre:  true
        // post: constructor from the set of creator files on 'fileNames' and sets the current scene

        // [[ DESTRUCTORS ]]

        // [[ MODIFIERS ]]
        void update();
        // pre:  true
        // post: updates every enabled and not paused gameobject of the current NTree
        void changeScene(const int &scene);
        // pre:  0 <= 'scene' < lenght of NTree list
        // post: changes the current NTree

        // [[ CONSULTORS ]]
        int changeScene() const;
        // pre:  true
        // post: return the current scene position on the list
        bool WindowShouldClose() const;
        // pre:  true
        // post: returns if the window should close
        void draw();
        // pre:  true
        // post: draws every enabled gameobject of the current NTree

    protected:
        bool _shouldClose;
        int _actual;

        vector<shared_ptr<NTree<GameObject>>> _rootlist;
        map<string, shared_ptr<Texture2D>> _texturelist;
        map<string, shared_ptr<Font>> _fontlist;

        void updatePostorder(const NTree<GameObject> &a);
        // pre:  true
        // post: if the content of the root of 'a' is enabled and not paused, updates the content 
        //       of the root of 'a' and recursivelly calls itself for all the childs of 'a'
        void drawPostorder(const NTree<GameObject> &a);
        // pre:  true
        // post: if the content of the root of 'a' is enabled, draws the content of the 
        //       root of 'a' and recursivelly calls itself for all the childs of 'a'
        bool exists(const NTree<GameObject> &a, string name);
        // pre:  true
        // post: recusivelly searches for a content in a where its name is 'name'
        NTree<GameObject> getNextGameObject(ifstream &file, bool &end);
        // pre:  true
        // post: adds to the last root on the list, content from the file
};
#endif