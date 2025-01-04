main:
	mkdir -p build
	g++ main.cpp -o build/main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall
	build/main 