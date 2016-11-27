#include "AstNative.h"

AstNative* AstNative::make(AstString* file_name, AstIdentifier* id, Expression* body)
{
	AstNative* n = new AstNative(file_name, id, body);
	Expression* res = get_exp(n);
	assert(res->get_type() == AST_NATIVE);
	return static_cast<AstNative*>(res);
}


AstNative::AstNative(AstString* file_name, AstIdentifier* id, Expression* body)
{
  this->et = AST_NATIVE;
  this->id = id;
  this->file_name = file_name;
  this->body = body;
  this->hash_c = 1 + id->get_hash() + 6*file_name->get_hash() + 484*body->get_hash();
}

string AstNative::to_string(int d)
{
  string res =  get_depth(d) + "Native " + id->get_id() + "\n";
  res +=  get_depth(d) + "FILENAME\n";
  res +=  file_name->to_string(d+1);
  res +=  get_depth(d) + "BODY\n";
  res +=  body->to_string(d+1);
  return res;
}

string AstNative::to_value()
{
  return "native " + file_name->to_value() + " as " + id->to_value() + " in " + body->to_value();
}

AstIdentifier* AstNative::get_id()
{
    return id;
}

AstString* AstNative::get_file_name()
{
    return file_name;
}

Expression* AstNative::get_body()
{
    return body;
}

Expression* AstNative::substitute(Expression* e1, Expression* e2)
{
	//avoid capture of bound variable
	if(e1 == id) return this;
	Expression* _new_id = id->substitute(e1, e2);
	assert(_new_id->get_type() == AST_IDENTIFIER);
	AstIdentifier* new_id = static_cast<AstIdentifier*>(_new_id);
	Expression* _new_file_name = file_name->substitute(e1, e2);
	AstString* new_file_name = static_cast<AstString*>(_new_file_name);
	Expression* new_body = body->substitute(e1, e2);
	return AstNative::make(new_file_name, new_id, new_body);
}

bool AstNative::operator==(const Expression& other)
{
	if(other.get_type() != AST_NATIVE) return false;
	AstNative& n = (AstNative&) other;
	return id == n.id && file_name == n.file_name && body == n.body;
}
