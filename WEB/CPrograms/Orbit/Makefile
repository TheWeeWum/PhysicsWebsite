CC = gcc
CFlags = -Wall -std=gnu99 -g

all: orbit

orbit: orbit.c orbit.h
	${CC} ${CFlags} $^ -o $@


.PHONY: clean
clean: 
	rm orbit.exe