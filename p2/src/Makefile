GLIB=$(shell pkg-config --cflags --libs glib-2.0)


all: build

build:
	yacc -d -v my_yacc.y -o y.tab.c
	lex lex.l
	gcc lex.yy.c y.tab.c -o parser -ly -lfl
	rm lex.yy.c
	rm y.tab.c
clean:
	rm parser

lex:
	lex lex.l
	gcc -o scanner lex.yy.c -lfl
	rm lex.yy.c
