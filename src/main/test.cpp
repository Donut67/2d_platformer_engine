#include "../include/conditionalGraph.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

void showGraph(const ConditionalGraph<int, string> &lg);

int main(){
    ConditionalGraph<int, string> lg;

    lg.AddVertex(100);
    lg.AddVertex(200);
    lg.AddVertex(300);
    lg.AddVertex(50);

    lg.AddEdge(100, 300, "first");
    lg.AddEdge(300, 200, "second");
    lg.AddEdge(100, 200, "third");
    lg.AddEdge(100,  50, "forth");

    cout << lg.Content() << "\n";

    for(auto i : lg.GetActualEdges()) {
        if(i == "third") {
            lg.GetDestination(i);
            break;
        }
    }

    cout << lg.Content() << "\n";

    

    return 0;
}

void showGraph(const ConditionalGraph<int, string> &lg){
    vector<int> val = lg.GetNodes();

    for(int v : val) {
        vector<string> labels = lg.GetEdges(v);
        cout << v << ": ";
        for(string l : labels) cout << l << " ";
        cout << "\n";
    }
}