#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "raylib.h"
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

template <class vertex>
class Graph {
    public:
        Graph(){}
        virtual void AddVertex(const vertex &v) = 0;
        virtual void AddEdge(const vertex &v1, const vertex &v2) = 0;
        virtual void RemoveVertex(const vertex &v) = 0;
        virtual void RemoveEdge(const vertex &v1, const vertex &v2) = 0;
        virtual bool ExistsVertex(const vertex &v) const = 0;
        virtual bool ExistsEdge(const vertex &v1, const vertex &v2) const = 0;
        virtual bool isEmpty() const = 0;
        virtual bool isFull() const = 0;
};
#endif