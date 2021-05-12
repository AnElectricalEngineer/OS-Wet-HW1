# Makefile for the smash program
# FOR C++
CC=g++
CFLAGS= -Wall -Werror -pedantic-errors -DNDEBUG *.cpp
CXXFLAGS=-std=c++11
CCLINK=$(CC)
OBJS=smash.o commands.o signals.o
RM=rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) $(CXXFLAGS) -o smash $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.h
smash.o: smash.cpp signals.h commands.h
signals.o: signals.cpp signals.h commands.h
# Cleaning old files before new make
#clean:
#	rm -f $(PROG) $(OBJS) $(TARGET) *.o *~ "#"* core.*
.PHONY: clean
clean:
	rm -f smash $(OBJS)
	$(RM) $(TARGET) *.o *~ "#"* core.*

