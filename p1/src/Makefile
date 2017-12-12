GLIB=$(shell pkg-config --cflags --libs glib-2.0)


all: build

build:
	lex mylex.l
	gcc -o scanner lex.yy.c -lfl
	rm lex.yy.c
clean:
	rm scanner
