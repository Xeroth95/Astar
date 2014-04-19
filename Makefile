CC = -Wall -Werror
LIBS = -lncurses

bin/main: main.c load.c load.h map.c map.h
	gcc ${CC} ${LIBS} main.c load.c map.c -o bin/main

run: bin/main
	bin/main
