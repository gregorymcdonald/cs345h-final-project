
#include "Evaluator.h"

#include "ast/expression.h"

#include "native/native.h"

/*
 * Author(s): Alex Meed, Brian Zhu, Greg McDonald, Patrick Moore
 * eid(s): grm695
 */



/*
 * Call this function to report any run-time errors
 * This will abort execution.
 */
void report_error(Expression* e, const string & s)
{
	cout << "Run-time error in expression " << e->to_value() << endl;
	cout << s << endl;
	exit(1);
}



Evaluator::Evaluator()
{
	sym_tab.push();
	c = 0;
}

Expression* Evaluator::eval_unop(AstUnOp* b)
{

	Expression* e = b->get_expression();
	Expression* eval_e = eval(e);

	Expression* result = NULL;
	switch(b->get_unop_type()){
		case PRINT:
		{
			if(eval_e->get_type() == AST_STRING) {
				AstString* s = static_cast<AstString*>(eval_e);
				cout << s->get_string() << endl;
			}
			else cout << eval_e->to_value() << endl;
			result = AstInt::make(0);
			break;
		}
		case ISNIL:
		{
			if(eval_e->get_type() == AST_NIL){
				result = AstInt::make(1);
			} else {
				result = AstInt::make(0);
			}
			break;
		}
		case HD: 
		{
			if(eval_e->get_type() == AST_LIST){
				AstList* list = static_cast<AstList*>(eval_e);
				result = list->get_hd();
			} else {
				// Should this just be e?
				result = eval_e;
			}
			break;
		}
		case TL:
		{
			if(eval_e->get_type() == AST_LIST){
				AstList* list = static_cast<AstList*>(eval_e);
				result = list->get_tl();
			} else {
				result = AstNil::make();
			}
			break;
		}
		default:
		{
			result = NULL;
			break;
		}
	}

	return result;
}

Expression* Evaluator::eval_binop(AstBinOp* b){
	// Get typing and expressions
	binop_type operation = b->get_binop_type();
	Expression* left_exp = Evaluator::eval(b->get_first());
	Expression* right_exp = Evaluator::eval(b->get_second());
	expression_type left_type = left_exp->get_type();
	expression_type right_type = right_exp->get_type();

	// Runtime errors
	bool illegalBinopTypes = (left_type != AST_INT && left_type != AST_STRING && left_type != AST_LIST && right_type != AST_INT && right_type != AST_STRING && right_type != AST_LIST);
	if ((left_type == AST_NIL || right_type == AST_NIL) && operation != CONS) {
		const string& errorMessage = "Nil can only be used with binop @";
        report_error(b, errorMessage);
	} else if ((left_type == AST_STRING && right_type == AST_STRING) && !(operation == PLUS || operation == EQ || operation == NEQ || operation == CONS)) {
		const string& errorMessage = "Binop " + AstBinOp::binop_type_to_string(operation)  + " cannot be applied to strings";
        report_error(b, errorMessage);
	} else if ((left_type == AST_LIST || right_type == AST_LIST) && operation != CONS) {
		const string& errorMessage = "Binpo @ is the only legal binop for lists";
        report_error(b, errorMessage);
	} else if((operation != CONS && left_type != right_type) || illegalBinopTypes){
        const string& errorMessage = "Binop can only be applied to expressions of same type";
        report_error(b, errorMessage);
	}

	// Evaluate the binop expression
	Expression* result = NULL;
	if(operation == CONS){
		if(right_exp->get_type() != AST_NIL){
			result = AstList::make(left_exp, right_exp);
		} else {
			result = left_exp;
		}
	} else {
		bool has_integer_args = left_type == AST_INT && right_type == AST_INT;
		bool has_string_args = left_type == AST_STRING && right_type == AST_STRING;

		long int left_int_value;
		long int right_int_value;
		string left_string_value;
		string right_string_value;
		if(has_integer_args){
			AstInt* left_int = static_cast<AstInt*>(left_exp);
			AstInt* right_int = static_cast<AstInt*>(right_exp);
			left_int_value = left_int->get_int();
			right_int_value = right_int->get_int();	
		} else if (has_string_args){
			AstString* left_string = static_cast<AstString*>(left_exp);
            AstString* right_string = static_cast<AstString*>(right_exp);
            left_string_value = left_string->get_string();
            right_string_value = right_string->get_string();
		} else {
			// cout << "ERROR: Something went wrong evaluating binop types" << endl;
			return NULL;
		}	

		switch(operation){
			case PLUS:
			{
				if(has_integer_args){
					result = AstInt::make(left_int_value + right_int_value);
				} else {
					result = AstString::make(left_string_value + right_string_value);
				}
				break;
			}
			case MINUS:
            {
                result = AstInt::make(left_int_value - right_int_value);
				break;
            }
			case TIMES:
            {
                result = AstInt::make(left_int_value * right_int_value);
                break;
            }
			case DIVIDE:
            {
                result = AstInt::make(left_int_value / right_int_value);
                break;
            }
            case EQ:
            {
            	if(has_integer_args){
            		result = AstInt::make( (left_int_value == right_int_value) ? 1 : 0 );
            	} else {
            		result = AstInt::make( (left_string_value.compare(right_string_value) == 0) ? 1 : 0 );
            	}
                break;
            }
            case NEQ:
            {
            	if(has_integer_args){
            		result = AstInt::make( (left_int_value != right_int_value) ? 1 : 0 );
            	} else {
            		result = AstInt::make( (left_string_value.compare(right_string_value) != 0) ? 1 : 0 );
            	}
                break;
            }
            case LT:
            {
            	result = AstInt::make( (left_int_value < right_int_value) ? 1 : 0 );
                break;
            }
            case LEQ:
            {
            	result = AstInt::make( (left_int_value <= right_int_value) ? 1 : 0 );
                break;
            }
            case GT:
            {
            	result = AstInt::make( (left_int_value > right_int_value) ? 1 : 0 );
                break;
            }
            case GEQ:
            {
            	result = AstInt::make( (left_int_value >= right_int_value) ? 1 : 0 );
                break;
            }
            case AND:
            {
            	result = AstInt::make( (left_int_value != 0 && right_int_value != 0) ? 1 : 0 );
                break;
            }
            case OR:
            {
            	result = AstInt::make( (left_int_value == 1 || right_int_value == 1) ? 1 : 0 );
                break;
            }
			default:
			{
				return NULL;
				break;
			}
		}
	}

	return result;
}

// XXX make native applications work here
Expression* Evaluator::eval_expression_list(AstExpressionList* l)
{
	Expression* result = NULL;

	vector<Expression*> expressions = l->get_expressions();
    unsigned int numExpressions = expressions.size();
	if(numExpressions < 2) { 
		// cout << "Error: cannot apply less than two expressions" << endl; 
		return NULL;
	}

    if(numExpressions == 2){
        // cout << "Single Case" << endl;
        // Single case
		Expression* e1 = Evaluator::eval(expressions[0]);
        if(e1->get_type() == AST_LAMBDA){
			AstLambda* e1_lambda = static_cast<AstLambda*>(e1);

        	result = Evaluator::eval(e1_lambda->get_body()->substitute(e1_lambda->get_formal(), expressions[1]));
		} else if (e1->get_type() == AST_DOUBLECOLON){
			cout << "Evaluating native function: 2 expressions case" << endl;
			
			// Trivial example
			int res = call_native<int>("trivial", "identity", 17);
			cout << res << endl;

			result = NULL;
		} else {
			const string& errorMessage = "Only lambda/native expressions can be applied to other expressions";
            report_error(l, errorMessage);
			result = NULL;
		}
    } else {
        // Multi case
        Expression* first_expression = expressions[0];

        if(first_expression->get_type() == AST_LAMBDA){
        	unsigned int index = 1;
	        Expression* partial_result = first_expression;
	        while(index < numExpressions){
				vector<Expression*>* two_expressions = new vector<Expression*>();
				two_expressions->push_back(partial_result);
				two_expressions->push_back(expressions[index]);
	            AstExpressionList* two_exp_application = AstExpressionList::make(*two_expressions);
	            partial_result = eval_expression_list(two_exp_application);
	            ++index;
	        }
	        result = partial_result;
    	} else if (first_expression->get_type() == AST_DOUBLECOLON){
			cout << "Evaluating native function: >2 expressions case" << endl;
			// XXX
			result = NULL;
		} else {
			const string& errorMessage = "Only lambda/native expressions can be applied to other expressions";
            report_error(l, errorMessage);
			result = NULL;
		}
    } 
	return result;
}


Expression* Evaluator::eval(Expression* e){
	Expression* res_exp = NULL;
	switch(e->get_type())
	{
	
	case AST_DOUBLECOLON:
    {
		// XXX Remove debug printing
		cout << "Evaluating Double Colon" << endl;
        
		//AstDoubleColon* dc = static_cast<AstDoubleColon*>(e);
		// XXX determine if any evaluation is needed for doublecolon
		res_exp = e;
        break;
    }
	case AST_NATIVE:
    {
		// XXX Remove debug printing
        cout << "Evaluating Native" << endl;
		
        AstNative* native = static_cast<AstNative*>(e);
        sym_tab.push();
		// Add a mapping from the id to this AstNative, may be a bad idea
		// Alternatively want a NativeModule Expression
        sym_tab.add(native->get_id(), e);
        Expression* e2 = Evaluator::eval(native->get_body());
        sym_tab.pop();

        res_exp = e2;
		
        break;
    }
	case AST_UNOP:
	{
		AstUnOp* b = static_cast<AstUnOp*>(e);
		res_exp = eval_unop(b);
		break;
	}
	case AST_READ:
	{
		AstRead* r = static_cast<AstRead*>(e);
		string input;
		getline(cin, input);
		if(r->read_integer()) {
			return AstInt::make(string_to_int(input));
		}
		return AstString::make(input);


		break;
	}
	case AST_INT:
	{
		res_exp = e;
		break;
	}
	case AST_STRING:
	{
		res_exp = e;
		break;
	}
	case AST_IDENTIFIER:
	{
		AstIdentifier* id = static_cast<AstIdentifier*>(e);
		res_exp = sym_tab.find(id);

		if(res_exp == NULL){
			// Reference parser has an extra whitespace at the end
			const string& errorMessage = "Identifier " + id->get_id()  + " is not bound in current context";
			report_error(e, errorMessage);
		}

		break;
	}
	case AST_LET:
    {
        AstLet* let = static_cast<AstLet*>(e);
		Expression* e1 = Evaluator::eval(let->get_val());
		sym_tab.push();
		sym_tab.add(let->get_id(), e1);
		Expression* e2 = Evaluator::eval(let->get_body());
		sym_tab.pop();

		res_exp = e2;

        break;
    }
	case AST_BRANCH:
    {
       	AstBranch* if_then_else = static_cast<AstBranch*>(e);
		Expression* pred = Evaluator::eval(if_then_else->get_pred());

		if(pred->get_type() != AST_INT){
			const string& errorMessage = "Predicate in conditional must be an integer";
            report_error(e, errorMessage);
		}

		long int predValue = static_cast<AstInt*>(pred)->get_int();
		if(predValue != 0){
			res_exp = Evaluator::eval(if_then_else->get_then_exp());
		} else {
			res_exp = Evaluator::eval(if_then_else->get_else_exp());
		} 

        break;
    }
	case AST_LAMBDA:
    {
        res_exp = e;
        break;
    }
	case AST_EXPRESSION_LIST:
    {
        AstExpressionList* expression_list = static_cast<AstExpressionList*>(e);
		res_exp = eval_expression_list(expression_list);
		
        break;
    }
	case AST_BINOP:
    {
        AstBinOp* binop = static_cast<AstBinOp*>(e);
		res_exp = Evaluator::eval_binop(binop);
        break;
    }
	case AST_NIL:
    {
        res_exp = e;
        break;
    }
	case AST_LIST:
    {
        res_exp = e;
        break;
    }
	case AST_IDENTIFIER_LIST:
    {
        res_exp = e;
        break;
    }

	default:
		assert(false);

	}
	return res_exp;
}
