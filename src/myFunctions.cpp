#include "myFunctions.h"
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

string ftostr(float f, int precision) {
    stringstream stream;
    stream << fixed << setprecision(precision) << f;
    return stream.str();
}

unsigned char stouc(const string &source){
    return (unsigned char)stoi(source);
}

string slice(string source, char divider, int &pos){
    int prev = pos, last = pos;
    bool found = false;

    while(!found && last <= (int)source.length()){
        found = source[last] == divider;
        if(!found) last ++;
    }

    pos = last + 1;
    // cout<<"sliced word: prev: \""<<prev<<"\", last: \""<<last<<"\"\t"<<source.substr(prev, last - prev)<<"\n";
    return source.substr(prev, last - prev);
}

vector<string> splice(string source, char divider){
    vector<string> result;
    int pos = 0;

    while(pos <= (int)source.length()) result.push_back(slice(source, divider, pos));

    return result;
}