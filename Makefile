OS=$(shell uname -s)
CFLAGS=-Wall -Werror -Wextra
TEST_FLAGS=-lcheck -lm -lrt -lsubunit -lpthread -pthread
CC=gcc

LIBS=-lcheck
ifeq ($(OS), Darwin)
	TEST_FLAGS=
	LIBS+=-largp
endif

run: build
	./server.out $(ARGS)

build: clean server.o http.o cnet.o thpool.o
	$(CC) ./server.o ./cnet.o ./http.o ./thpool.o -o server.out $(LIBS) $(CFLAGS) -pthread

rebuild: clean build

test: build
	checkmk clean_mode=1 ./src/test.check > ./test.c
	$(CC) ./cnet.o ./http.o thpool.o ./test.c -o ./test.out $(LIBS) $(TEST_FLAGS)
	./test.out

docker-test:
	docker exec -it server make test

create_lib: build
	ar rcs libmylib.a cnet.o http.o

server.o: ./src/server.c ./src/parser_args/parser_args.h
	$(CC) -c ./src/server.c

cnet.o: ./src/cnet_lib/cnet.c ./src/cnet_lib/cnet.h
	$(CC) -c ./src/cnet_lib/cnet.c

http.o: ./src/http_lib/http.c ./src/http_lib/http.h
	$(CC) -c ./src/http_lib/http.c

thpool.o: ./src/thread_pool/thpool.c ./src/thread_pool/thpool.h
	$(CC) -c ./src/thread_pool/thpool.c

clean:
	rm -rf *.a *.o *.out ./test.c

# goals for test multi-threading
run_multi: build_multi
	./server.out

build_multi:
	$(CC) -D MULTI_TH \
	./src/server.c ./src/cnet_lib/cnet.c ./src/http_lib/http.c ./src/thread_pool/thpool.c \
	-o server.out $(LIBS) $(CFLAGS) -pthread
