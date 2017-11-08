all: resize copy temp

resize: resize.c bmp.h
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o resize resize.c -lm

copy: copy.c bmp.h
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o copy copy.c -lm

temp: temp.c bmp.h
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o temp temp.c -lm

clean:
	rm copy resize temp