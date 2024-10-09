CXX      := -gcc
CXXFLAGS := -Wall -Wextra -Werror
LDFLAGS  := -lstdc++ -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
EXEC_DIR  := $(BUILD)/
TARGET   := executable
INCLUDE  := -Iinclude/
SRC      :=  $(wildcard src/*.c)

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(EXEC_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
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