main: main.c load.c load.h map.c map.h
	gcc -lncurses main.c load.c map.c -o main
