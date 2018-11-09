raytracer: main.cpp Scene.cpp Vector.cpp
	g++ -o raytracer main.cpp Scene.cpp Vector.cpp -std=c++11 -lpthread -lX11
