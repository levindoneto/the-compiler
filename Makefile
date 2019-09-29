etapa2: y.tab.o lex.yy.o hash.o main.o
	gcc -o etapa3 y.tab.o lex.yy.o main.o hash.o

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
y.tab.o: y.tab.c
	gcc -c y.tab.c
hash.o: hash.c
	gcc -c hash.c
main.o: main.c
	gcc -c main.c
lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l
y.tab.c: parser.y
	yacc -d parser.y
clean:
	rm lex.yy.c etapa3 *.o y.tab.c y.tab.h lex.yy.h
