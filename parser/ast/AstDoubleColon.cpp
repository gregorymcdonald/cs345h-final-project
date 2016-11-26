#include "AstDoubleColon.h"

AstDoubleColon* AstDoubleColon::make(const string & left_id, const string & right_id)
{
	AstDoubleColon* d = new AstDoubleColon(left_id, right_id);
	Expression* res = get_exp(d);
	assert(res->get_type() == AST_DOUBLECOLON);
	return static_cast<AstDoubleColon*>(res);
}

AstDoubleColon::AstDoubleColon(const string & left_id, const string & right_id)
{
	this->et = AST_DOUBLECOLON;
    this->left_id = AstIdentifier::make(left_id);
	this->right_id = AstIdentifier::make(right_id);
    this->hash_c = 8484 + left_id.size() + (left_id.size()>0? left_id[0]:0)+
    		(right_id.size()>1? right_id[1]:0) + (right_id.size()>2? right_id[2]:0);
}

string AstDoubleColon::to_string(int d)
{
  return get_depth(d) + this->to_value() + "\n";
}
string AstDoubleColon::to_value()
{
  return this->left_id->to_value() + "::" + this->right_id->to_value();
}

AstIdentifier* AstDoubleColon::get_left_id()
{
    return left_id;
}

AstIdentifier* AstDoubleColon::get_right_id()
{
    return right_id;
}

Expression* AstDoubleColon::substitute(Expression* e1, Expression* e2)
{
	if(e1 == this) return e2;
	// XXX implement more expressive substitution if necessary
	return this;
}


bool AstDoubleColon::operator==(const Expression& other)
{
	if(other.get_type() != AST_DOUBLECOLON) return false;
	AstDoubleColon& o = (AstDoubleColon&) other;
	return left_id == o.left_id && right_id == o.right_id;
}

