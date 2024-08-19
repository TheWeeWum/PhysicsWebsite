CC = gcc
CFlags = -Wall -std=gnu99 -g

all: fluid

fluid: fluid.c fluid.h
	${CC} ${CFlags} $^ -O3 -o $@
 

.PHONY: clean
clean: 
	rm fluid.exe