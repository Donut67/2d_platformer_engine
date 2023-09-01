default: 
	.\build/engine.exe
game:
	g++ -std=c++17 src/main/game.cpp src/*.cpp -o game.exe -O2 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
	./game.exe
engine: 
	g++ -std=c++17 main.cpp platformerEngineBasics.cpp platformerEngineUI.cpp -o engine.exe -O2 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
	./engine.exe
test:
	g++ -std=c++17 src/main/test.cpp -o test.exe -O2 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
	./test.exe