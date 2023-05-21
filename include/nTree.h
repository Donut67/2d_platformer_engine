#ifndef __N_TREE_H__
#define __N_TREE_H__

#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

template<class element>
class NTree {
    public:
        // CONSTRUCTORS
        NTree(){
        // pre:  -
        // post: The tree is empty
            _root = nullptr;
        }
        NTree(const NTree &a, shared_ptr<element> c){
        // pre:  true (error: bad_alloc)
        // post: generates a tree where c is the root and a is the first son
            _root = shared_ptr<Node>(new Node); // bad_alloc
            _root->_content = c;
            _root->_brother = nullptr;
            _root->_child = a._root;
        }

        // DESTRUCTORS
        void remove() {
            if(_root != nullptr) {
                cout << _root->_content->name() << "\n";
                _root = nullptr;
            }
        }

        // MODIFIERS
        void setBrother(const NTree &a){
        // pre:  true
        // post: sets a as the furthest brother 
            shared_ptr<Node> previous = nullptr;
            shared_ptr<Node> actual = _root;

            while (actual != nullptr) {
                previous = actual;
                actual = actual->_brother;
            }

            if (previous == nullptr) _root = a._root;
            else previous->_brother = a._root;
        }
        void setChild(const NTree &a){
        // pre:  true
        // post: sets a as the furthest child 
            shared_ptr<Node> previous = nullptr;
            shared_ptr<Node> actual = _root->_child;

            while (actual != nullptr) {
                previous = actual;
                actual = actual->_brother;
            }

            if (previous == nullptr) _root->_child = a._root;
            else previous->_brother = a._root;
        }

        // CONSULTORS
        bool isEmpty() const{
        // pre:  true
        // post: checks if the tree is empty
            return _root == nullptr;
        }
        shared_ptr<element> content() const{
        // pre:  true (error: arbre buit)
        // post: returns a copy of the GameObject on the root of the tree
            shared_ptr<element> value;

            if (!isEmpty()) value = _root->_content;

            return value;
        }

        int nChilds() const{
        // pre:  true (error: arbre buit)
        // post: returns the number of children the root has
            int value = 0;

            if (!isEmpty()) {
                shared_ptr<Node> aux = _root->_child;
                while (aux != nullptr) {
                    value++;
                    aux = aux->_brother;
                }
            }

            return value;
        }
        int nBrothers() const{
        // pre:  true (error: arbre buit)
        // post: returns the number of brothers the root has
            int value = 0;
            shared_ptr<Node> aux = _root;

            while (aux != nullptr) {
                value++;
                aux = aux->_brother;
            }

            return value;
        }

        NTree find(const string &name) const{
            NTree res;
            int i = 1;
            bool found = false;

            while(!found && i <= nBrothers()){
                res = find_i(brother(i), found, name);
                i++;
            }
            
            return res;
        }

        NTree child(int n) const{
        // pre:  n >= 0 (error: tree empty)
        // post: returns the i child of the root, empty tree if there are less childs
            NTree value;

            if (!isEmpty()) {
                int i = 1;
                shared_ptr<Node> aux = _root->_child;

                while (aux != nullptr && i < n) {
                    i++;
                    aux = aux->_brother;
                }

                value._root = aux;
            }

            return value;
        }
        
        NTree brother(int n) const{
        // pre:  n >= 0 (error: tree empty)
        // post: returns the i brother of the root, empty tree if there are less brothers
            int i = 1;
            NTree value;
            shared_ptr<Node> aux = _root;

            while (aux != nullptr && i < n) {
                aux = aux->_brother;
                i++;
            }

            value._root = aux;
            return value;
        }

    private:
        struct Node {
            shared_ptr<element> _content;
            shared_ptr<Node> _child, _brother;
        };
        
        shared_ptr<Node> _root;

        NTree find_i(const NTree &a, bool &found, const string &name) const{
            NTree aux;
            if(!a.isEmpty() && a.content()->name() != name){
                int i = 1;
                while(!found && i <= a.nChilds()){
                    aux = find_i(a.child(i), found, name);
                    i++;
                }
            } else if(!a.isEmpty() && a.content()->name() == name){
                found = true;
                aux = a;
            }
            return aux;
        }
};
#endif