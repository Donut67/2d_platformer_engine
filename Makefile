default:
	g++ -std=c++17 src/main/game.cpp src/*.cpp -o build/game.exe -O2 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
	.\build/game.exe
# default: 
# 	g++ -std=c++17 src/main/engine.cpp src/*.cpp -o build/engine.exe -O2 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
# 	.\build/engine.exe