#ifndef __LABELED_GRAPH_H__
#define __LABELED_GRAPH_H__

#include "raylib.h"
#include "graph.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

template <class vertex, class label>
class LabeledGraph : public Graph<vertex> {
    public:
        LabeledGraph() {
            _first = nullptr;
        }

        void AddVertex(const vertex &v){
            if(!ExistsVertex(v)){
                shared_ptr<node> newNode = make_shared<node>();
                newNode->content = v;
                newNode->next  = nullptr;
                newNode->edges = nullptr;

                if(isEmpty()) _first = newNode;
                else {
                    shared_ptr<node> aux2 = _first;
                    while(aux2->next != nullptr) aux2 = aux2->next;

                    aux2->next = newNode;
                } 
            }
        }
        void AddEdge(const vertex &v1, const vertex &v2){
            AddEdge(v1, v2, label());
        }
        void AddEdge(const vertex &v1, const vertex &v2, const label &l){
            shared_ptr<Edge> edge = findEdge(v1, v2);

            if(edge != nullptr) edge->lab = l;
            else {
                shared_ptr<node> aux1 = findVertex(v1);
                shared_ptr<node> aux2 = findVertex(v2);
                
                shared_ptr<Edge> newEdge = make_shared<Edge>();
                newEdge->dest = aux2;
                newEdge->next = nullptr;
                newEdge->lab = l;

                if(aux1->edges != nullptr) {
                    shared_ptr<Edge> edges = aux1->edges;
                    while(edges->next != nullptr) edges = edges->next;

                    edges->next = newEdge;
                } else aux1->edges = newEdge;
            }
        }
        void RemoveVertex(const vertex &v){

        }
        void RemoveEdge(const vertex &v1, const vertex &v2){
            shared_ptr<node> aux1 = findVertex(v1);
            shared_ptr<node> aux2 = findVertex(v2);

            shared_ptr<Edge> edges = aux1->edges;
            shared_ptr<Edge> prev  = aux1->edges;
            bool found = false;

            while(edges != nullptr) {
                found = edges->dest == aux2;
                if(!found){
                    prev = edges;
                    edges = edges->next;
                }
            }

            prev->next = edges->next;
            // delete edges;
        }

        bool ExistsVertex(const vertex &v) const{
            return findVertex(v) != nullptr;
        }
        bool ExistsEdge(const vertex &v1, const vertex &v2) const{
            return findEdge(v1, v2) != nullptr;
        }
        bool isEmpty() const{
            return _first == nullptr;
        }
        bool isFull() const{
            return false;
        }
        
        vector<vertex> GetNodes() const{
            shared_ptr<node> aux = _first;
            vector<vertex> result;

            while(aux != nullptr) {
                result.push_back(aux->content);
                aux = aux->next;
            }

            return result;
        }
        vector<label> GetEdges(const vertex &v) const {
            shared_ptr<node> aux = findVertex(v);
            shared_ptr<Edge> edges = aux->edges;
            vector<label> result;

            while(edges != nullptr) {
                result.push_back(edges->lab);
                edges = edges->next;
            }

            return result;
        }
        vertex GetDestination(const vertex &v, const label &l) const{
            shared_ptr<node> aux = findVertex(v);
            shared_ptr<Edge> edges = aux->edges;
            bool found = false;

            while(edges != nullptr && !found) {
                found = edges->lab == l;
                if(!found) edges = edges->next;
            }

            return edges->dest->content;
        }
        label GetLabel(const vertex &v1, const vertex &v2) const{
            return findEdge(v1, v2)->lab;
        }
        
    protected:
        struct Edge; // oops!
        struct node {
            vertex content;
            shared_ptr<node> next;
            shared_ptr<Edge> edges;
        };
        struct Edge {
            label lab;
            shared_ptr<node> dest;
            shared_ptr<Edge> next;
        };
        shared_ptr<node> findVertex(const vertex &v) const{
            shared_ptr<node> aux = _first;
            bool found = false;

            while(aux != nullptr && !found) {
                found = aux->content == v;
                if(!found) aux = aux->next;
            }

            return found? aux : nullptr;
        }
        shared_ptr<Edge> findEdge(const vertex &v1, const vertex &v2) const{
            shared_ptr<node> aux1 = findVertex(v1);
            shared_ptr<node> aux2 = findVertex(v2);

            shared_ptr<Edge> edges = aux1->edges;
            bool found = false;

            while(edges != nullptr && !found) {
                found = edges->dest == aux2;
                if(!found) edges = edges->next;
            }

            return edges;
        }
        // void copy(const LabeledGraph &o);

        shared_ptr<node> _first;
};
#endif