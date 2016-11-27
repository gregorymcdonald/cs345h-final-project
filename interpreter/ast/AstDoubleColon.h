 #ifndef AST_DOUBLECOLON_H_
 #define AST_DOUBLECOLON_H_
 
 #include "Expression.h"
 #include "AstIdentifier.h" 

 class AstDoubleColon: public Expression
 {
   private:
     AstIdentifier* left_id;
	 AstIdentifier* right_id;
     AstDoubleColon(const string & left_id, const string & right_id);
   public:
    static AstDoubleColon* make(const string & left_id, const string & right_id);
    virtual string to_string(int d = 0);
    virtual string to_value();
    AstIdentifier* get_left_id();
	AstIdentifier* get_right_id();
	virtual Expression* substitute(Expression* e1,
	        		  Expression* e2);
	virtual bool operator==(const Expression& other);

    
 };
 
 #endif /* AST_DOUBLECOLON_H_ */
