#include "../include/labeledGraph.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

void showGraph(const LabeledGraph<int, string> &lg);

int main(){
    LabeledGraph<int, string> lg;

    lg.AddVertex(100);
    lg.AddVertex(200);
    lg.AddVertex(300);
    lg.AddVertex(50);

    lg.AddEdge(100, 300, "first");
    lg.AddEdge(300, 200, "second");
    lg.AddEdge(100, 200, "third");
    lg.AddEdge(100,  50, "forth");

    showGraph(lg);

    cout << lg.getDestination(100, "forth") << "\n";

    return 0;
}

void showGraph(const LabeledGraph<int, string> &lg){
    vector<int> val = lg.getNodes();

    for(int v : val) {
        vector<string> labels = lg.getEdges(v);
        cout << v << ": ";
        for(string l : labels) cout << l << " ";
        cout << "\n";
    }
}