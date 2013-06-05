CC=g++
CFLAGS=-Wall -O2 -std=gnu++11
DEBUG_FLAGS=-Wall -O0 -g
.SUFFIXES = .cpp
SRC_DIR = src
OBJ_DIR = obj
SRCS:=$(wildcard $(SRC_DIR)/*.cpp)
OBJS:=$(SRCS:.cpp=.o)
TARGET = backpropagation

.PHONY: all clean debug

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET): $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJS))
	$(CC) $(CFLAGS) -o $@ $^

debug: $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJS))
	$(CC) $(DEBUG_FLAGS) -o $(TARGET).out $^

clean:
	$(RM) -r $(OBJ_DIR) $(TARGET) $(TARGET).out

$(OBJ_DIR)/mylib.o: $(patsubst %,$(SRC_DIR)/%, mylib.hpp BackPropagation.hpp )
$(OBJ_DIR)/BackPropagation.o: $(patsubst %,$(SRC_DIR)/%,BackPropagation.hpp)
$(OBJ_DIR)/BackPropagation_setter_and_getter.o: $(patsubst %,$(SRC_DIR)/%,BackPropagation.hpp)