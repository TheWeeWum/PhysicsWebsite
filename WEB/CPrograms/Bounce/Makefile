CC = gcc
CFlags = -Wall -std=gnu99 -g

all: bounce

bounce: bounce.c bounce.h
	${CC} ${CFlags} $^ -o $@


.PHONY: clean
clean: 
	rm bounce.exe