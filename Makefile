CC=g++
CFLAGS=-std=c++11 -Wall
OBJS=main.o voter.o graph.o

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS)

voter.o: voter.h
graph.h: graph.h voter.h

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
