CXX = g++

gui: gui.cpp
	$(CXX) -o build/gui -lX11 gui.cpp

screenshot: screenshot.cpp
	$(CXX) -o build/screenshot -lX11 -lpthread screenshot.cpp
