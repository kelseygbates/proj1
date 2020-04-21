
OBJS = myshell.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

myshell : $(OBJS)
    $(CC) $(LFLAGS) $(OBJS) –o myshell

myshell.o : myshell.h
    $(CC) $(CFLAGS) myshell.cpp

clean:
    \rm *.o *~