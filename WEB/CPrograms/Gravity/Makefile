CC = gcc
CFlags = -Wall -std=gnu99 -g

all: gravsim

gravsim: gravsim.c gravsim.h
	${CC} ${CFlags} $^ -o $@


.PHONY: clean
clean: 
	rm gravsim.exe