CC=g++
CFLAGS=-Wall
.SUFFIXES = .cpp

backpropagation: backpropagation.o mylib.o
	$(CC) $(CFLAGS) -o backpropagation backpropagation.cpp mylib.cpp

.cpp.o:
	$(CC) $(CFLAGS) -c $<

mylib.o:mylib.h

.PHONY: clean
