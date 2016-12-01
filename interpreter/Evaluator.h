

#ifndef EVALUATOR_H_
#define EVALUATOR_H_

class Expression;
class AstBinOp;
class AstUnOp;
class AstExpressionList;
struct lni_object;
#include "SymbolTable.h"

class Evaluator {
private:
	SymbolTable sym_tab;
	int c;
public:
	Evaluator();
	Expression* eval(Expression* e);
	Expression* eval_binop(AstBinOp* b);
	Expression* eval_unop(AstUnOp* b);
	Expression* eval_expression_list(AstExpressionList* l);

	lni_object* convert_expression_to_lni_object(Expression* e);
	Expression* convert_lni_object_to_expression(lni_object* l);
};

#endif /* EVALUATOR_H_ */
