CC=g++
CFLAGS=-Wall -O3
.SUFFIXES = .cpp
objs:=$(wildcard *.o)

backpropagation: backpropagation.o mylib.o Net.o struct.o Net_setter_and_getter.o
	$(CC) $(CFLAGS) -o backpropagation backpropagation.o mylib.o Net.o struct.o Net_setter_and_getter.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

mylib.o:mylib.h struct.h Net.h
Net.o:Net.h struct.h
Net_setter_and_getter.o:Net.h struct.h
struct.o:struct.h

.PHONY: clean
clean:
	$(RM) $(objs)
