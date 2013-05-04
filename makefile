CC=g++
CFLAGS=-Wall
.SUFFIXES = .cpp

backpropagation: backpropagation.o mylib.o bp.o
	$(CC) $(CFLAGS) -o backpropagation backpropagation.o mylib.o bp.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

bp.o:bp.h
mylib.o:mylib.h

.PHONY: clean
