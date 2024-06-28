OBJS = Arqade.cpp AudioPlayer.cpp Core.cpp CoreData.cpp VideoPlayer.cpp Utils.cpp
BUILD_DIR = bin
OBJ_NAME = Arqade
CC = g++
C_FLAGS = -Werror
DEBUG_FLAGS = -g -DDEBUG
LN_FLAGS = -lSDL2

all: dir $(OBJS)
	$(CC) $(OBJS) $(C_FLAGS) $(LN_FLAGS) -o $(BUILD_DIR)/$(OBJ_NAME)

debug: dir $(OBJS)
	$(CC) $(DEBUG_FLAGS) $(OBJS) $(C_FLAGS) $(LN_FLAGS) -o $(BUILD_DIR)/$(OBJ_NAME)

dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm $(OBJ_NAME)
