CXX      := gcc
CXXFLAGS := -Wall -Wextra -Werror
LDFLAGS  := -lstdc++ -lm -lpthread
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
EXEC_DIR := $(BUILD)/
TARGET   := executable
INCLUDE  := -Iinclude/
SRC      := $(wildcard src/*.c)  # Corrigido para C, não C++

OBJECTS := $(SRC:%.c=$(OBJ_DIR)/%.o)  # Corrigido para C, não C++

all: build $(EXEC_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.c  # Corrigido para C, não C++
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(EXEC_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(EXEC_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release run

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

run:
	./$(BUILD)/$(TARGET)
