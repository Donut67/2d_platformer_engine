#ifndef __ENGINE_LABELED_GRAPH_H__
#define __ENGINE_LABELED_GRAPH_H__

#include "raylib.h"
#include "window.h"
#include "engineStateWindow.h"
#include "labeledGraph.h"
#include <cstdlib>
#include <iostream>
#include <functional>
#include <map>

using namespace std;

// using function<bool()> condition;
template<typename N>
struct condition {
    // function<bool(N, N)> _op;
    N _first, _second;
    condition() {}
    condition (/*function<bool(N, N)> op, */N first, N second) : _first(first), _second(second){

    }
    bool operator == (condition<N> c){
        return _first == c._first && _second == c._second;
    }
    bool something() { 
        return _op(_first, _second);
    }
};

struct Line {
    Vector2 _begin, _end;
    
    Line(Vector2 begin, Vector2 end) {
        _begin = begin;
        _end = end;
    }

    void moveBegin(Vector2 pos){ _begin = Vector2{pos.x + _begin.x, pos.y + _begin.y}; }
    void moveEnd(Vector2 pos){ _end = Vector2{pos.x + _end.x, pos.y + _end.y}; }

    void draw() {
        DrawLineEx(_begin, _end, 2, RED);
    }
};

class WindowLabeledGraph : public Window {
    public:
        WindowLabeledGraph() : Window("", Vector2{0,0}, Vector2{0,0}){}
        WindowLabeledGraph(Vector2 pos) : Window("Animator", pos, Vector2{750.0f, 550.0f}) {
            _labeled_graph = make_shared<LabeledGraph<string, condition<int>>>();
            _labeled_graph->AddVertex("Idle");
            _labeled_graph->AddVertex("Walk");
            _labeled_graph->AddVertex("Jump");
            _labeled_graph->AddVertex("Run");
            _labeled_graph->AddVertex("Crouch");

            _labeled_graph->AddEdge("Idle", "Walk", condition<int>(0, 0));
            _labeled_graph->AddEdge("Walk", "Idle", condition<int>(0, 0));
            _labeled_graph->AddEdge("Idle", "Jump", condition<int>(1, 0));

            int i = 0;
            for(string item : _labeled_graph->GetNodes()){
                vector<condition<int>> conditions = _labeled_graph->GetEdges(item);

                vector<ConditionButtonData> options;
                for(condition<int> con : conditions) {
                    options.push_back(ConditionButtonData(
                        string(to_string(con._first) + " > " + to_string(con._second)),
                        [](){cout << "clicked\n";},
                        [](){cout << "selected\n";}
                    ));
                }
                _states.push_back(make_shared<EngineStateWindow>(options, item, Vector2{pos.x + 5, pos.y + 50 + 40 * i}, Vector2{150, 25.0f + 25 * conditions.size()}, 18, false));

                i++;
            }

            i = 0;
            for(string item : _labeled_graph->GetNodes()){
                int j = 0;
                for(condition<int> con : _labeled_graph->GetEdges(item)) {
                    string dest = _labeled_graph->GetDestination(item, con);
                    Vector2 begin, end;
                    begin = end = Vector2{pos.x + 5 + 150 - 15, pos.y + 50 + 40 * i + 35 + 30 * j};

                    int x = 0;
                    bool found = false;
                    while(x < _states.size() && !found) {
                        found = _states[x]->getTitle() == dest;
                        if(!found) x++;
                    }

                    if(found) end = _states[x]->getPosition();
                    _connections.push_back(make_shared<Line>(begin, end));
                    _start[item].push_back(_connections.back());
                    _destination[dest].push_back(_connections.back());
                    j++;
                }

                i++;
            }
        }

        void update();

        void draw();

    private:
        shared_ptr<LabeledGraph<string, condition<int>>> _labeled_graph;
        vector<shared_ptr<EngineStateWindow>> _states;
        vector<shared_ptr<Line>> _connections;

        map<string, vector<shared_ptr<Line>>> _start, _destination;
};

#endif