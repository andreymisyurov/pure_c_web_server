build:
	gcc ./src/server.c -o server.out -largp && ./server.out

run:
	./src/server.out

build_test:
	checkmk clean_mode=1 str.check >tests.c && gcc -Wall -Werror -Wextra -lcheck s21_string.c tests.c && ./a.out
