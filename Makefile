etapa2: main.o hash.o lex.yy.o y.tab.o
	gcc main.o hash.o lex.yy.o y.tab.o -o etapa2
main.o: main.c
	gcc -c main.c
hash.o: hash.c
	gcc -c hash.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
y.tab.o: y.tab.c
	gcc -c y.tab.c
lex.yy.c: y.tab.c scanner.l
	lex scanner.l
y.tab.c: parser.y
	yacc -d parser.y
clean:
	rm lex.yy.c etapa1
