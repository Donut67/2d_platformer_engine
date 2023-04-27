#ifndef __CONDITIONAL_GRAPH_H__
#define __CONDITIONAL_GRAPH_H__

#include "raylib.h"
#include "labeledGraph.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

template <class vertex, class label>
class ConditionalGraph : public LabeledGraph<vertex, label> {
    public:
        ConditionalGraph() : LabeledGraph<vertex, label>() {
            _actual = this->_first;
        }
        
        void AddVertex(const vertex &v){
            bool empty = LabeledGraph<vertex, label>::isEmpty();
            LabeledGraph<vertex, label>::AddVertex(v);
            if(empty) _actual = this->_first;
        }
        
        vector<label> GetActualEdges() const {
            return LabeledGraph<vertex, label>::GetEdges(Content());
        }
        vertex GetDestination(const label &l) {
            vertex res = LabeledGraph<vertex, label>::GetDestination(Content(), l);
            _actual = LabeledGraph<vertex, label>::findVertex(res);
            return res;
        }
        vertex Content() const {
            return _actual->content;
        }

    private:
        shared_ptr<typename LabeledGraph<vertex, label>::node> _actual;
};
#endif