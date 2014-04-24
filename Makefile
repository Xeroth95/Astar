VERSION = 0.04a
CC = gcc
CFLAGS = -Wall -Werror -g -DVERSION=\"$(VERSION)\"
LDFLAGS = -lncurses
OBJ_PATH = obj
BIN_PATH = bin
OUTPUT = main

OBJ = $(OBJ_PATH)/main.o $(OBJ_PATH)/load.o $(OBJ_PATH)/map.o $(OBJ_PATH)/astar.o

$(BIN_PATH)/$(OUTPUT): ${OBJ}
	$(CC) $(CFLAGS) -o $(BIN_PATH)/$(OUTPUT) $(OBJ) $(LDFLAGS)

$(OBJ_PATH)/%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN_PATH)/$(OUTPUT)
	$(BIN_PATH)/$(OUTPUT)

clean:
	rm $(OBJ_PATH)/*
	rm $(BIN_PATH)/*
