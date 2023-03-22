#ifndef __MY_FUNCTIONS_H__
#define __MY_FUNCTIONS_H__

#include <vector>
#include <string>

using namespace std;

unsigned char stouc(const string &source);
string slice(string source, char divider, int &pos);
vector<string> splice(string source, char divider);

#endif