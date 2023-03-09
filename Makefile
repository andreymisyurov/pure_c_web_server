CC=gcc
FLAGS=-Wall -Werror -Wextra

run: build
	./server.out $(ARGS)

build: server http cnet
	$(CC) ./server.o ./cnet.o ./http.o -o server.out -largp

test: build
	checkmk clean_mode=1 ./src/test.check > ./src/test.c
	$(CC) $(FLAGS) ./cnet.o ./http.o ./src/test.c -lcheck -largp -o ./test.out
	./test.out

create_lib: build
	ar rcs libmylib.a cnet.o http.o

server:
	$(CC) -c ./src/server.c

cnet:
	$(CC) -c ./src/cnet_lib/cnet.c

http:
	$(CC) -c ./src/http_lib/http.c

clean:
	rm -rf *.a *.o *.out
