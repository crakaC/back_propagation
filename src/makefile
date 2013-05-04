CC=g++
CFLAGS=-Wall
.SUFFIXES = .cpp

backpropagation: backpropagation.o mylib.o bp.o Net.o struct.o
	$(CC) $(CFLAGS) -o backpropagation backpropagation.o mylib.o bp.o Net.o struct.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

bp.o:bp.h struct.h
mylib.o:mylib.h struct.h
Net.o:Net.h struct.h
struct.o:struct.h

.PHONY: clean
	rm *.o *~
