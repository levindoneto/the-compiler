etapa2: lex.yy.c
	gcc lex.yy.c -o etapa2
lex.yy.c: y.tab.c scanner.l
	lex scanner.l
y.tab.c:
	yacc -d parser.y
clean:
	rm lex.yy.c etapa2 *.o y.tab.c y.tab.h
