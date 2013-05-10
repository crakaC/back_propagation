CC=g++
CFLAGS=-Wall -O0
.SUFFIXES = .cpp
SRC_DIR = src
BUILD_DIR = bin
SRCS:=$(wildcard $(SRC_DIR)/*.cpp)
OBJS:=$(SRCS:.cpp=.o)
TARGET = backpropagation

.PHONY: all clean

all: $(BUILD_DIR) $(patsubst %,$(BUILD_DIR)/%,$(TARGET))

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/$(TARGET): $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(OBJS))
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) -r $(BUILD_DIR)

$(BUILD_DIR)/mylib.o: $(patsubst %,$(SRC_DIR)/%, mylib.hpp struct.hpp Net.hpp )
$(BUILD_DIR)/Net.o: $(patsubst %,$(SRC_DIR)/%,Net.hpp struct.hpp)
$(BUILD_DIR)/Net_setter_and_getter.o: $(patsubst %,$(SRC_DIR)/%,Net.hpp struct.hpp)
$(BUILD_DIR)/struct.o: $(patsubst %,$(SRC_DIR)/%,struct.hpp)