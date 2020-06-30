GCC = gcc -g -c
OBJ = server/build/main.o server/build/functions.o server/build/ffmpeg_module.o
OBJ_TEST = server/build/test.o server/build/functions.o

.PHONY: clean

all: server/build main_server prog_test

server/build/main.o: server/main.c
	$(GCC) server/main.c -o server/build/main.o

server/build/functions.o: server/functions.c
	$(GCC) server/functions.c -o server/build/functions.o

server/build/ffmpeg_module.o: server/ffmpeg_module.c
	$(GCC) server/ffmpeg_module.c -o server/build/ffmpeg_module.o

server/build/test.o: server/test/test.c
	$(GCC) -I server -c server/test/test.c -o server/build/test.o

prog_test: $(OBJ_TEST)
	gcc -g $(OBJ_TEST) -o prog_test

main_server: $(OBJ)
	gcc -g $(OBJ) -o main_server

server/build:
	mkdir server/build

clean:
	rm -rf server/build main_server prog_test