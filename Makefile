etapa5: main.o hash.o lex.yy.o y.tab.o ast.o semantic.o tac.o
	gcc main.o hash.o lex.yy.o y.tab.o ast.o semantic.o tac.o -o etapa5

main.o: main.c
	gcc -c main.c

ast.o: ast.c
	gcc -c ast.c

hash.o: hash.c
	gcc -c hash.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

semantic.o: semantic.c
	gcc -c semantic.c

lex.yy.c: y.tab.c scanner.l
	lex scanner.l

y.tab.c: parser.y
	yacc -d parser.y

tac.c: tac.c
	gcc -c tac.c

clean:
	rm *.o etapa5 lex.yy.c y.tab.c
