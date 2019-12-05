etapa4: main.o hash.o lex.yy.o y.tab.o ast.o semantic.o
	gcc main.o hash.o lex.yy.o y.tab.o ast.o semantic.o -o etapa4

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

clean:
	rm *.o etapa4 lex.yy.c y.tab.c
