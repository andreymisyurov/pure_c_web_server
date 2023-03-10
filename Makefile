OS=$(shell uname -s)
CFLAGS=-Wall -Werror -Wextra
TEST_FLAGS=-lcheck -lm -lrt -lsubunit -lpthread
CC=gcc

LIBS=-lcheck
ifeq ($(OS), Darwin)
	TEST_FLAGS=
	LIBS+=-largp
endif

run: build
	./server.out $(ARGS)

build: server.o http.o cnet.o
	$(CC) ./server.o ./cnet.o ./http.o -o server.out $(LIBS) $(CFLAGS)

test: build
	checkmk clean_mode=1 ./src/test.check > ./src/test.c
	$(CC) ./cnet.o ./http.o ./src/test.c -o ./test.out $(LIBS) $(TEST_FLAGS)
	./test.out

docker-test:
	docker exec -it server make test

create_lib: build
	ar rcs libmylib.a cnet.o http.o

server.o:
	$(CC) -c ./src/server.c

cnet.o:
	$(CC) -c ./src/cnet_lib/cnet.c

http.o:
	$(CC) -c ./src/http_lib/http.c

clean:
	rm -rf *.a *.o *.out
