CC = g++
CFLAGS = -g -Wall -std=c++11
CXXFLAGS = -g -Wall -std=c++11
INC=-. ./ast ./native
INC_PARAMS=$(foreach d, $(INC), -I$d)

OBJs =  lex.yy.o parser.tab.o Expression.o SymbolTable.o frontend.o Evaluator.o AstRead.o AstNil.o AstList.o AstUnOp.o AstBranch.o AstExpressionList.o AstIdentifierList.o AstBinOp.o  AstIdentifier.o AstInt.o AstLambda.o AstLet.o AstString.o AstDoubleColon.o AstNative.o
default: parser

parser: ${OBJs}
	${CC} ${CXXFLAGS} ${INC_PARAMS} ${OBJs} -o l-interpreter -lfl -ldl

frontend.o:	frontend.cpp Evaluator.cpp 
	${CC} ${CXXFLAGS} ${INC_PARAMS} -c frontend.cpp Evaluator.cpp 
	
SymbolTable.o:	SymbolTable.cpp
	${CC} ${CXXFLAGS} ${INC_PARAMS} -c SymbolTable.cpp 

Expression.o:	ast/*.h ast/*.cpp #ast/Expression.cpp ast/Expression.h ast/AstString.cpp ast/AstString.h
	${CC} ${CXXFLAGS} ${INC_PARAMS} -c ast/*.cpp


clean:
	rm -f l-interpreter  parser.output *.o

depend:
	makedepend -I. *.c
