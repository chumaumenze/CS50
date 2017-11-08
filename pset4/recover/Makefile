all: recover

recover: recover.c jpeg.h
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o recover recover.c -lm

clean:
	rm -f recover *.jpg *.jpeg