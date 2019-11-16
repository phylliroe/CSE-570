all: calc.lex calc.yacc lex.yy.c y.tab.c
	lex calc.lex
	yacc -d calc.yacc
	cc lex.yy.c y.tab.c -o project

