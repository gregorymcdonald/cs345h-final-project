CC = g++
CFLAGS = -g -Wall -std=c++11
CXXFLAGS = -g -Wall -std=c++11
INC=-. ./ast ./native
INC_PARAMS=$(foreach d, $(INC), -I$d)

OBJs =   parser.tab.o lex.yy.o  Expression.o SymbolTable.o frontend.o Evaluator.o AstRead.o AstNil.o AstList.o AstUnOp.o AstBranch.o AstExpressionList.o AstIdentifierList.o AstBinOp.o  AstIdentifier.o AstInt.o AstLambda.o AstLet.o AstString.o AstDoubleColon.o AstNative.o native.o
default: parser

parser: ${OBJs}
	${CC} ${CXXFLAGS} ${INC_PARAMS} ${OBJs} -o l-interpreter -lfl

lex.yy.o: lex.yy.c

parser.tab.o: parser.tab.c  parser-defs.h

native.o: native/native.cc
	${CC} ${CXXFLAGS} ${INC_PARAMS} -c native/native.cc -ldl

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
