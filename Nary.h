#ifndef __N_ARY_TREE_H__
#define __N_ARY_TREE_H__

// #include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

template<class element>
class Nary{
    private:
        struct Node {
            shared_ptr<element> _content;
            shared_ptr<Node> _parent, _brother, _child;
        };

        shared_ptr<Node> _root;

        Nary find_i(const Nary &a, bool &found, shared_ptr<element> e) const{
            Nary aux;
            if(!a.isEmpty() && a.content() != e){
                int i = 1;
                while(!found && i <= a.nChilds()){
                    aux = find_i(a.child(i), found, e);
                    i++;
                }
            } else if(!a.isEmpty() && a.content() == e){
                found = true;
                aux = a;
            }
            return aux;
        }
    public:
        Nary() {
            _root = nullptr;
        }
        Nary(shared_ptr<element> e) {
            _root = make_shared<Node>();
            _root->_content = e;
            _root->_parent  = nullptr;
            _root->_brother = nullptr;
            _root->_child   = nullptr;
        }
        Nary(shared_ptr<element> e, const Nary &a) {
            _root = make_shared<Node>();
            _root->_content = e;
            _root->_parent  = a._root;
            _root->_brother = nullptr;
            _root->_child   = nullptr;
        }

        void setParent(const Nary &a) {
            _root->_parent = a._root;
        }
        void setBrother(const Nary &a) {
            shared_ptr<Node> previous = nullptr;
            shared_ptr<Node> actual = _root;

            while (actual != nullptr) {
                previous = actual;
                actual = actual->_brother;
            }

            if (previous == nullptr) {
                _root = a._root;
                // _root->_parent = a.root->_parent;
            } else {
                previous->_brother = a._root;
                previous->_brother->_parent = _root->_parent;
            }
        }
        void setChild(const Nary &a) {
            shared_ptr<Node> previous = nullptr;
            shared_ptr<Node> actual = _root->_child;

            while (actual != nullptr) {
                previous = actual;
                actual = actual->_brother;
            }

            if (previous == nullptr) {
                _root->_child = a._root;
                _root->_child->_parent = _root;
            } else {
                previous->_brother = a._root;
                previous->_brother->_parent = _root;
            }
            // cout << "child setted\n";
        }

        bool isEmpty() const {
            return _root == nullptr;
        }
        
        Nary parent() const {
            Nary value;

            if(_root->_parent != nullptr) value._root = _root->_parent;
            
            return value;
        }
        Nary brother(const int &n) const {
            int i = 1;
            Nary value;
            shared_ptr<Node> aux = _root;

            while (aux != nullptr && i < n) {
                aux = aux->_brother;
                i++;
            }

            value._root = aux;
            return value;
        }
        Nary child(const int &n) const {
            Nary value;

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

        Nary find(shared_ptr<element> e) const {
            Nary res;
            int i = 1;
            bool found = false;

            while(!found && i <= nBrothers()){
                res = find_i(brother(i), found, e);
                i++;
            }
            
            return res;
        }

        int nBrothers() const {
            int value = 0;
            shared_ptr<Node> aux = _root;

            while (aux != nullptr) {
                value++;
                aux = aux->_brother;
            }

            return value;
        }
        int nChilds() const {
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
        
        shared_ptr<element> content() const {
            return _root->_content;
        }
};

#endif