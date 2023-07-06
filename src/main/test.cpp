#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main() {
    std::cout << fs::current_path() << '\n';
    std::string path = fs::current_path().string() + "\\resources";
    for (const auto & entry : fs::recursive_directory_iterator(path)){
        std::string text = entry.path().string();
        if(entry.path().extension() == ".meta") std::cout << text.substr(text.find("resources"), text.size()) << std::endl;
    }
}