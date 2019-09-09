yacc -d parser.y;
lex scanner.l;
gcc lex.yy.c -o etapa2;
