#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream> 
#include <vector>
#include <stdio.h>

using namespace std;

void createMonoFile(string name);
void createComponentFile(string name);
string nextWord(string line, int &pos, bool &ended);

int main(int argc, char **argv){
    if(argc >= 2 && argc <= 3){
        if(argv[1][0] == '-'){
            if(argv[1][1] == 'h') cout<<"There should be only 1 argument:\n\t-h (help)\n\tclass name (normal class name with spaces)\n";
            else if(argv[1][1] == 'm'){
                string filename;
                cout<<"ENTER FILENAME FOR MONO BEHAVIOR CLASS:\n";
                getline(cin, filename);
                createMonoFile(filename);
            }else if(argv[1][1] == 'c'){
                //string filename;
                //cout<<"ENTER FILENAME FOR COMPONENT CLASS:\n";
                //getline(cin, filename);
                //createComponentFile(filename);
            }
        }
    }else cout<<"There should be only 1 argument:\n\thelp\n\tclass name\n";

    return 0;
}

void createMonoFile(string name){
    bool ended = false;
    int pos = 0;
    string defineClass, className, fileName;

    while(!ended && pos < name.length()) {
        string aux1 = nextWord(name, pos, ended);
        string aux2 = aux1;

        aux1[0] = toupper(aux1[0]);
        className += aux1;

        for (int x=0; x<aux2.length(); x++) aux2[x] = toupper(aux2[x]);
        defineClass += aux2;
        if(!ended) defineClass += "_";
    }
    
    fileName = className;
    fileName[0] = tolower(fileName[0]);
    
    {
        string file = "include/" + fileName + ".h";
        ofstream outfile(file, ios::binary);
        outfile << "#ifndef __" << defineClass <<"_H__\n#define __"<< defineClass <<"_H__\n\n";
        outfile << "#include \"raylib.h\"\n#include \"gameObject.h\"\n#include \"monoBehavior.h\"\n#include <cstdlib>\n#include <iostream>\n";
        outfile << "\nusing namespace std;\n\nclass GameObject;\nclass MonoBehavior;\n\n";
        outfile << "class " << className <<" : public MonoBehavior{\n    public:\n        // CONSTRUCTORS\n        "<< className <<"();\n\n";
        outfile << "        // DESTRUCTORS\n\n        // MODIFIERS\n        void setGameObject(shared_ptr<GameObject> go);\n\n";
        outfile << "        void update();\n\n        // CONSULTORS\n        void draw();\n\n    private:\n\n};\n#endif";
    }
    {
        string file = fileName + ".cpp";
        ofstream outfile(file, ios::binary);
        outfile << "#include \"include/raylib.h\"\n#include \"include/" << fileName << ".h\"\n#include <cstdlib>\n#include <iostream>\n";
        outfile << "\nusing namespace std;\n\n" << className << "::" << className << "(){\n\n}\n\n";
        outfile << "void " << className << "::setGameObject(shared_ptr<GameObject> go){\n    _gameObject = go;\n}\n\n";
        outfile << "void " << className << "::update(){\n\n}\n\n";
        outfile << "void " << className << "::draw(){\n\n}";
    }
    {
        ifstream outfile1("scene.cpp", ios::binary);
        ofstream outfile2("_scene.cpp", ios::binary);
        string createfile = name;
        for(int i = createfile.find(' '); i < createfile.length() - 1; i++) createfile[i] = createfile[i + 1];
        createfile.pop_back();
        if (outfile1.is_open()) {
            int i = 0;
            string line;
            while(outfile1){
                getline(outfile1, line);
                outfile2<<line;
                if(i == 13) outfile2<<"    else if(comp == \""<<createfile<<"\") object->addComponent(make_shared<"<<className<<">("<<className<<"()));\n";
                i++;
            }
        }
        outfile1.close();
        outfile2.close();
        remove("scene.cpp");
        rename("_scene.cpp", "scene.cpp");
    }

    ofstream bulk_file("include/bulkfile.h", ios::app);
    bulk_file << "\n#include \"" << fileName << ".h\"";
}

void createComponentFile(string name){
    bool ended = false;
    int pos = 0;
    string defineClass, className, fileName;

    while(!ended && pos < name.length()) {
        string aux1 = nextWord(name, pos, ended);
        string aux2 = aux1;

        aux1[0] = toupper(aux1[0]);
        className += aux1;

        for (int x=0; x<aux2.length(); x++) aux2[x] = toupper(aux2[x]);
        defineClass += aux2;
        if(!ended) defineClass += "_";
    }
    
    fileName = className;
    fileName[0] = tolower(fileName[0]);
    
    {
        string file = "include/" + fileName + ".h";
        ofstream outfile(file, ios::binary);
        outfile << "#ifndef __" << defineClass <<"_H__\n#define __"<< defineClass <<"_H__\n\n";
        outfile << "#include \"raylib.h\"\n#include \"gameObject.h\"\n#include \"component.h\"\n#include <cstdlib>\n#include <iostream>\n";
        outfile << "\nusing namespace std;\n\nclass GameObject;\nclass Component;\n\n";
        outfile << "class " << className <<" : public Component{\n    public:\n        // CONSTRUCTORS\n        "<< className <<"();\n\n";
        outfile << "        // DESTRUCTORS\n\n        // MODIFIERS\n        void setGameObject(shared_ptr<GameObject> go);\n\n";
        outfile << "        void update();\n\n        // CONSULTORS\n        void draw();\n\n    private:\n\n};\n#endif";
    }
    {
        string file = fileName + ".cpp";
        ofstream outfile(file, ios::binary);
        outfile << "#include \"include/raylib.h\"\n#include \"include/" << fileName << ".h\"\n#include <cstdlib>\n#include <iostream>\n";
        outfile << "\nusing namespace std;\n\n" << className << "::" << className << "(){\n\n}\n\n";
        outfile << "void " << className << "::setGameObject(shared_ptr<GameObject> go){\n    _gameObject = go;\n}\n\n";
        outfile << "void " << className << "::update(){\n\n}\n\n";
        outfile << "void " << className << "::draw(){\n\n}";
    }

    ofstream bulk_file("include/bulkfile.h", ios::app);
    bulk_file << "\n#include \"include/" << fileName << ".h\"";
}

string nextWord(string line, int &pos, bool &ended){
    bool found = false;
    int end = pos, prev = pos;

    while(!found && end < line.length()){
        found = line[end] == ' ';
        if(!found) end++;
    }

    ended = end == line.length();
    pos = end + 1;

    return line.substr(prev, end);
}