VERSION = 0.04a
CC = gcc
CFLAGS = -Wall -Werror -g -DVERSION=\"$(VERSION)\"
LDFLAGS = -lncurses

OBJ = main.o load.o map.o astar.o

bin/main: ${OBJ}
	$(CC) $(CFLAGS) -o bin/main $(OBJ) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: bin/main
	bin/main

clean:
	rm $(OBJ)
