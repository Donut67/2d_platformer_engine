#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "raylib.h"
#include "nTree.h"
#include "component.h"
#include "includefile.h"
#include <cstdlib>
#include <iostream>
#include <variant>
#include <vector>
#include <memory>

using namespace std;

class Scene;
class Component;

class GameObject : public enable_shared_from_this<GameObject>{
    public:
        // [[ CONSTRUCTORS ]]
        GameObject();
        // pre:  true
        // post: default constructor
        GameObject(string name, shared_ptr<NTree<GameObject>> root);
        // pre:  true
        // post: constructor that sets the name of the game object and a pointer to the current NTree of its scene

        // [[ DESTRUCTORS ]]

        // [[ MODIFIERS ]]
        void closeWindow();
        // pre:  true
        // post: sets if the window should close
        void addComponent(shared_ptr<Component> comp);
        // pre:  'comp' exists
        // post: adds 'comp' to the back of the list of components of the game object
        void setEnable(const bool &enable);
        // pre:  'enabled' exists
        // post: sets if the game object is enabled by 'enabled'
        void setPaused(const bool &paused);
        // pre:  'paused' exists
        // post: sets if the game object is paused by 'paused'
        void update();
        // pre:  true
        // post: updates all the components of the game object in order

        // [[ CONSULTORS ]]
        string name() const;
        // pre:  true
        // post: returns the name of the game object
        bool isEnable() const;
        // pre:  true
        // post: returns if the game object is enabled
        bool isPaused() const;
        // pre:  true
        // post: returns if the game object is paused
        bool WindowShouldClose() const;
        // pre:  true
        // post: returns if the window should close
        vector<shared_ptr<Component>> getComponents() const;
        // pre:  true
        // post: returns the list of components of the gam object
        shared_ptr<Component> operator [](const int &pos) const;
        // pre:  0 <= pos < lenght of '_compList'
        // post: returns the component at 'pos' position
        template<typename N>
        shared_ptr<N> operator [](const N &type) const{
            // pre:  true
            // post: returns the first component that is of the type of 'type'
            shared_ptr<N> ret = NULL;
            for(auto i : _compList) if(auto j = dynamic_pointer_cast<N>(i)) return j;
            return ret;
        }
        shared_ptr<NTree<GameObject>> getRoot() const;
        // pre:  true
        // post: return the root of the actual scene NTree
        NTree<GameObject> getParent() const;
        // pre:  true
        // post: returns the parent of this game object, empty NTree if this is the root of the scene NTree
        NTree<GameObject> find_this() const;
        // pre:  true
        // post: returns an NTree which content is this
        NTree<GameObject> find(const string &name) const;
        // pre:  true
        // post: returns an NTree which content is a game object of name 'name', empty if it doesn't exists
        void draw() const;
        // pre:  true
        // post: draws all the components of the game object in order

    private:
        string _name;
        bool _enabled, _shouldClose, _isPaused;

        vector<shared_ptr<Component>> _compList;
        shared_ptr<NTree<GameObject>> _root;

        NTree<GameObject> getParent_i(const NTree<GameObject> &a, bool &found, bool &ended) const;
        // pre:  true
        // post: iterative function to find the parent of this game object
        NTree<GameObject> find_i(const NTree<GameObject> &a, bool &found, const string &name) const;
        // pre:  true
        // post: iterative function to find a game object of name 'name'
};
#endif