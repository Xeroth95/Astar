VERSION = 0.04a
CC = gcc
CFLAGS = -g -O3 -fstack-protector -W -Wall -Wno-unused-parameter -Wno-unused-function -Wno-unused-label -Wpointer-arith -Wformat -Wreturn-type -Wsign-compare -Wmultichar -Wformat-nonliteral -Winit-self -Wuninitialized -Wno-deprecated -Wformat-security -Werror
#-Wall -Werror -g -DVERSION=\"$(VERSION)\"
LDFLAGS = -lncurses
OBJ_PATH = obj
BIN_PATH = bin
OUTPUT = main

OBJ = $(OBJ_PATH)/main.o $(OBJ_PATH)/load.o $(OBJ_PATH)/map.o $(OBJ_PATH)/astar.o $(OBJ_PATH)/keyboard.o $(OBJ_PATH)/printer.o

$(BIN_PATH)/$(OUTPUT): ${OBJ}
	$(CC) $(CFLAGS) -o $(BIN_PATH)/$(OUTPUT) $(OBJ) $(LDFLAGS)

$(OBJ_PATH)/%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN_PATH)/$(OUTPUT)
	$(BIN_PATH)/$(OUTPUT)

clean:
	rm $(OBJ_PATH)/*
	rm $(BIN_PATH)/*

open_gl: $(OBJ_PATH)/openGL_printer.o
	$(CC) -o $(BIN_PATH)/test $< -lGL -lGLU -lGLEW -lglut 
	primusrun $(BIN_PATH)/test
