CC = gcc
CFlags = -Wall -std=gnu99 -g

all: powder

powder: powder.c powder.h
	${CC} ${CFlags} $^ -o $@


.PHONY: clean
clean: 
	rm powder.exe