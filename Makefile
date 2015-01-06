all: clean
	lex compiler.l
	yacc -d -v compiler.y
	cc lex.yy.c y.tab.c -o compiler -lfl

clean:
	rm -f compiler
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h