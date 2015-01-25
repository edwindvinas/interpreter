all: clean
	lex lexer.l
	yacc -d -v parser.y
	cc lex.yy.c y.tab.c nodes.c -o interpreter -lfl

clean:
	rm -f compiler
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h