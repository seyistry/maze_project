all:
	gcc src/*.c -lm `pkg-config --cflags --libs sdl2` -o maze_app