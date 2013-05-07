CC=g++
CFLAGS=-Wall -O3
.SUFFIXES = .cpp
objs:=$(wildcard *.o)

backpropagation: backpropagation.o mylib.o Net.o struct.o Net_setter_and_getter.o
	$(CC) $(CFLAGS) -o backpropagation backpropagation.o mylib.o Net.o struct.o Net_setter_and_getter.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

mylib.o:mylib.hpp struct.hpp Net.hpp
Net.o:Net.hpp struct.hpp
Net_setter_and_getter.o:Net.hpp struct.hpp
struct.o:struct.hpp

.PHONY: clean
clean:
	$(RM) $(objs)
