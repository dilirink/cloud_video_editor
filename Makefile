GCC = gcc -g -c
OBJ = server/build/main.o server/build/functions.o

.PHONY: clean

all: build server

server: main.o functions.o
	gcc -g $(OBJ) -o main_server

main.o: server/main.c server/functions.h
	$(GCC) server/main.c -o server/build/main.o

functions.o: server/functions.c server/functions.h
	$(GCC) server/functions.c -o server/build/functions.o

build:
	mkdir server/build

clean:
	rm -rf server/build main_server