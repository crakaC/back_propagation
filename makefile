CC=g++
CFLAGS=-Wall
.SUFFIXES = .cpp

backpropagation: backpropagation.o mylib.o bp.o Net.o
	$(CC) $(CFLAGS) -o backpropagation backpropagation.o mylib.o bp.o Net.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

bp.o:bp.h struct.h
mylib.o:mylib.h struct.h
Net.o:Net.h struct.h

.PHONY: clean
	rm *.o *~
