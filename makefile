CC=g++
CFLAGS=-Wall -O3
DEBUG_FLAGS=-Wall -O0 -g
.SUFFIXES = .cpp
SRC_DIR = src
OBJ_DIR = obj
SRCS:=$(wildcard $(SRC_DIR)/*.cpp)
OBJS:=$(SRCS:.cpp=.o)
TARGET = backpropagation

.PHONY: all clean dbg

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

$(OBJ_DIR)/mylib.o: $(patsubst %,$(SRC_DIR)/%, mylib.hpp struct.hpp Net.hpp )
$(OBJ_DIR)/Net.o: $(patsubst %,$(SRC_DIR)/%,Net.hpp struct.hpp)
$(OBJ_DIR)/Net_setter_and_getter.o: $(patsubst %,$(SRC_DIR)/%,Net.hpp struct.hpp)
$(OBJ_DIR)/struct.o: $(patsubst %,$(SRC_DIR)/%,struct.hpp)