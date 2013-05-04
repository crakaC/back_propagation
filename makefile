CC=g++
CFLAGS=-Wall
.SUFFIXES = .cpp

backpropagation: backpropagation.o mylib.o bp.o Net.o
	$(CC) $(CFLAGS) -o backpropagation backpropagation.o mylib.o bp.o Net.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

bp.o:bp.h
mylib.o:mylib.h
Net.o:Net.h

.PHONY: clean
