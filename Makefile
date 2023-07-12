CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wno-missing-braces -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

src = src/main/engine.cpp $(wildcard src/*.cpp)

rtc = #RuntimeCompiler/BuildTool.cpp RuntimeCompiler/Compiler_PlatformWindows.cpp RuntimeCompiler/FileChangeNotifier.cpp
sfw = #RuntimeCompiler/SimpleFileWatcher/FileWatcher.cpp RuntimeCompiler/SimpleFileWatcher/FileWatcherWin32.cpp

ros = #RuntimeObjectSystem/ObjectInterfacePerModuleSource.cpp RuntimeObjectSystem/RuntimeObjectSystem.cpp RuntimeObjectSystem/RuntimeObjectSystem_PlatformWindows.cpp
ofs = #RuntimeObjectSystem/ObjectFactorySystem/ObjectFactorySystem.cpp
osf = #RuntimeObjectSystem/SimpleSerializer/SimpleSerializer.cpp

inc = include/ #RuntimeCompiler/ RuntimeObjectSystem/ RuntimeCompiler/SimpleFileWatcher/ RuntimeObjectSystem/ RuntimeObjectSystem/ObjectFactorySystem/ RuntimeObjectSystem/SimpleSerializer/

default: 
	.\build/engine.exe
game:
	g++ -std=c++17 src/main/game.cpp src/*.cpp -o game.exe -O2 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
	./game.exe
engine: 
	g++ -std=c++17 RuntimeCompiler/*.cpp RuntimeCompiler/SimpleFileWatcher/*.cpp RuntimeObjectSystem/*.cpp RuntimeObjectSystem/ObjectFactorySystem/*.cpp RuntimeObjectSystem/SimpleSerializer/*.cpp src/main/engine.cpp src/platformerEngineBasics.cpp src/platformerEngineUI.cpp -o engine.exe -O2 -Wall -Wno-missing-braces -I include/ RuntimeObjectSystem/ RuntimeCompiler/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
	./engine.exe
test: 
	$(CXX) $(src) $(rtc) $(sfw) $(ros) $(ofs) $(osf) -o engine.exe -I $(inc) $(CXXFLAGS)
	./engine.exe