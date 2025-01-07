CC       := gcc
CXXFLAGS := -Wall -Wextra -Werror
LDFLAGS  := -lpthread -lstdc++ -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
EXEC_DIR := $(BUILD)
TARGET   := executable
INCLUDE  := -Iinclude/
SRC      := $(wildcard src/*.c)

OBJECTS := $(SRC:src/%.c=$(OBJ_DIR)/%.o)

all: build $(EXEC_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(@D) 
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(EXEC_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D) 
	$(CC) $(OBJECTS) $(CXXFLAGS) $(LDFLAGS) -o $@

build:
	@mkdir -p $(EXEC_DIR) 
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(EXEC_DIR)/*

run: all
	./$(EXEC_DIR)/$(TARGET)
