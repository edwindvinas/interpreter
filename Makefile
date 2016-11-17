all: clean
	lex lexer.l
	yacc -d -v parser.y
	cc lex.yy.c y.tab.c cmd.c interpreter.c nodes.c -o interpreter -lfl

clean:
	rm -f interpreter
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f y.output
	rm -f interpreter-posselt-sladkovsky.tar.gz

dist: clean
	tar -czvf interpreter-posselt-sladkovsky.tar.gz *