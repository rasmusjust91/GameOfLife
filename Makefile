SCR_DIR=scr
BUILD_DIR=build/debug
CC=/usr/bin/clang++
COMPILER_FLAGS=-std=c++17 -stdlib=libc++
SCR_FILES=$(wildcard $(SCR_DIR)/*.cpp) $(wildcard $(SCR_DIR)/utils/*.cpp)
OBJ_NAME=GameofLife
INCLUDE_PATHS=-Iinclude -Iscr/utils
LIBRARY_PATHS=-Llib
LINKER_FLAGS=-lsdl2

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SCR_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

run:
	./$(BUILD_DIR)/$(OBJ_NAME)