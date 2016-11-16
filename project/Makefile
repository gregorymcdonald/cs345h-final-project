CC = g++
CFLAGS = -g -Wall -std=c++0x
INC=-. ./ast
INC_PARAMS=$(foreach d, $(INC), -I$d)

OBJs =   parser.tab.o lex.yy.o  Expression.o SymbolTable.o frontend.o AstRead.o AstNil.o AstList.o AstUnOp.o AstBranch.o AstExpressionList.o AstIdentifierList.o AstBinOp.o  AstIdentifier.o AstInt.o AstLambda.o AstLet.o AstString.o 

default: lexer

lexer: ${OBJs}
	${CC} ${CFLAGS} ${INC_PARAMS} ${OBJs} -o lexer -lfl

lex.yy.c: lexer.l parser-defs.h
	flex -i lexer.l


frontend.o:	frontend.cpp 
	${CC} ${CFLAGS} ${INC_PARAMS} -c frontend.cpp
	
SymbolTable.o:	SymbolTable.cpp
	${CC} ${CFLAGS} ${INC_PARAMS} -c SymbolTable.cpp 

Expression.o:	ast/*.h ast/*.cpp 
	${CC} ${CFLAGS} ${INC_PARAMS} -c ast/*.cpp


clean:
	rm -f lexer lexer.yy.c parser *.o parser.tab.[ch] parser.output

clean1exer:
	rm -f lexer lexer.yy.c *.o parser.tab.[ch] parser.output

cleanparser:
	rm -f parser *.o parser.tab.[ch] parser.output

depend:
	makedepend -I. *.c 
