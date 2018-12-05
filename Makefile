CXX = g++
DEPS = screenshot.h clip/clip.h

main: main.cpp screenshot.cpp $(DEPS)
	$(CXX) -o build/main -lX11 -lpthread main.cpp screenshot.cpp

gui: gui.cpp
	$(CXX) -o build/gui -lX11 gui.cpp

screenshot: screenshot.cpp
	$(CXX) -o build/screenshot -lX11 -lpthread screenshot.cpp
