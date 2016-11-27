#ifndef AST_NATIVE_H_
#define AST_NATIVE_H_

#include "Expression.h"
#include "AstIdentifier.h"
#include "AstString.h"

class AstNative:public Expression
{
  private:
    AstIdentifier* id;
    AstString* file_name;
    Expression* body;
    AstNative(AstString* file_name, AstIdentifier* id, Expression* body);
  public:
    static AstNative* make(AstString* file_name, AstIdentifier* id, Expression* body);
    virtual string to_string(int d = 0);
    virtual string to_value();
    AstIdentifier* get_id();
    Expression* get_body();
    AstString* get_file_name();
    virtual Expression* substitute(Expression* e1,
    	        		  Expression* e2);
    virtual bool operator==(const Expression& other);
};

#endif /* AST_NATIVE_H_ */
