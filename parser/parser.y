%{
/* Greg McDonald (grm695@utexas.edu) */

#include "parser-defs.h"

int yylex();
extern int yy_scan_string(const char* c);
int yyerror(const char* p)
{ 
    if(parser_error_fn != NULL) {
      parser_error_fn("At line " + int_to_string(curr_lineno) + ": " + string(p));
    }
    return 1;
};

/* HELPER FUNCTIONS */
AstLambda* generateLambda(AstIdentifierList* id_list, Expression* body){
    const vector<AstIdentifier*> ids = id_list->get_ids();

    AstLambda *prev = AstLambda::make(ids[0], body);   
    for(unsigned int j = 1; j < ids.size(); j++){
        AstLambda *temp = prev;
        prev = AstLambda::make(ids[j], temp);
    }

    return prev;
}

%}
/* BISON Declarations */
%token 
TOKEN_READSTRING 
TOKEN_READINT 
TOKEN_PRINT 
TOKEN_ISNIL
TOKEN_HD 
TOKEN_TL 
TOKEN_CONS 
TOKEN_NIL 
TOKEN_DOT 
TOKEN_WITH 
TOKEN_LET 
TOKEN_PLUS 
TOKEN_MINUS 
TOKEN_IDENTIFIER 
TOKEN_TIMES 
TOKEN_DIVIDE 
TOKEN_INT 
TOKEN_LPAREN 
TOKEN_RPAREN 
TOKEN_AND 
TOKEN_OR 
TOKEN_EQ 
TOKEN_NEQ 
TOKEN_GT 
TOKEN_GEQ 
TOKEN_LT 
TOKEN_LEQ 
TOKEN_IF 
TOKEN_THEN 
TOKEN_ELSE 
TOKEN_LAMBDA 
TOKEN_FUN 
TOKEN_COMMA 
TOKEN_STRING 
TOKEN_ERROR 
TOKEN_IN

%nonassoc EXPR
%nonassoc TOKEN_PRINT
%left TOKEN_GEQ
%left TOKEN_LEQ
%left TOKEN_GT
%left TOKEN_LT
%left TOKEN_NEQ
%left TOKEN_EQ
%left TOKEN_OR
%left TOKEN_AND
%left TOKEN_MINUS
%left TOKEN_PLUS
%left TOKEN_DIVIDE
%left TOKEN_TIMES
%nonassoc TOKEN_ISNIL
%right TOKEN_CONS
%nonassoc TOKEN_TL
%nonassoc TOKEN_HD

%%

program: expression
{
    res_expr = $$;
}

expression:
TOKEN_LET TOKEN_IDENTIFIER TOKEN_EQ expression TOKEN_IN expression %prec EXPR
{
    string id_lexeme = GET_LEXEME($2);
    AstIdentifier *id =  AstIdentifier::make(id_lexeme);
    $$ = AstLet::make(id, $4, $6);
}
|
TOKEN_FUN TOKEN_IDENTIFIER TOKEN_WITH id_list TOKEN_EQ expression TOKEN_IN expression %prec EXPR
{
    string id_lexeme = GET_LEXEME($2);
    AstIdentifier *f_id =  AstIdentifier::make(id_lexeme);

    Expression *_i = $4;
    assert(_i->get_type() == AST_IDENTIFIER_LIST);
    AstIdentifierList* id_list = static_cast<AstIdentifierList*>(_i);

    AstLambda *l = generateLambda(id_list, $6);
    $$ = AstLet::make(f_id, l, $8);
}
|
TOKEN_LAMBDA id_list TOKEN_DOT expression %prec EXPR
{
    Expression *_i = $2;
    assert(_i->get_type() == AST_IDENTIFIER_LIST);
    AstIdentifierList* id_list = static_cast<AstIdentifierList*>(_i);

    /*
    const vector<AstIdentifier*> ids = id_list->get_ids();

    AstLambda *prev = AstLambda::make(ids[0], $4);
    
    for(unsigned int j = 1; j < ids.size(); j++){
        AstLambda *temp = prev;
        prev = AstLambda::make(ids[j], temp);
    }
    
    $$ = prev;
    */

    $$ = generateLambda(id_list, $4);
}
|
TOKEN_IF expression TOKEN_THEN expression TOKEN_ELSE expression %prec EXPR
{
    $$ = AstBranch::make($2, $4, $6);
}
|
expression TOKEN_PLUS expression
{
	 /* XXX Maybe need %prec EXPR at end */
    $$ = AstBinOp::make(PLUS, $1, $3);
}
|
expression TOKEN_AND expression
{
	$$ = AstBinOp::make(AND, $1, $3);
}
|
expression TOKEN_OR expression
{
    $$ = AstBinOp::make(OR, $1, $3);
}
|
expression TOKEN_MINUS expression
{
    $$ = AstBinOp::make(MINUS, $1, $3);
}
|
expression TOKEN_TIMES expression
{
    $$ = AstBinOp::make(TIMES, $1, $3);
}
|
expression TOKEN_DIVIDE expression
{
    $$ = AstBinOp::make(DIVIDE, $1, $3);
}
|
expression TOKEN_EQ expression
{
    $$ = AstBinOp::make(EQ, $1, $3);
}
|
expression TOKEN_NEQ expression
{
    $$ = AstBinOp::make(NEQ, $1, $3);
}
|
expression TOKEN_LT expression
{
    $$ = AstBinOp::make(LT, $1, $3);
}
|
expression TOKEN_LEQ expression
{
    $$ = AstBinOp::make(LEQ, $1, $3);
}
|
expression TOKEN_GT expression
{
    $$ = AstBinOp::make(GT, $1, $3);
}
|
expression TOKEN_GEQ expression
{
    $$ = AstBinOp::make(GEQ, $1, $3);
}
|
TOKEN_INT 
{
  	string lexeme = GET_LEXEME($1);
  	long int val = string_to_int(lexeme);
  	Expression* e = AstInt::make(val);
  	$$ = e;
} 
|
TOKEN_STRING 
{
	  string lexeme = GET_LEXEME($1);
  	Expression* e = AstString::make(lexeme);
  	$$ = e;
}  
|
TOKEN_LPAREN expression_application TOKEN_RPAREN
{
    $$ = $2;
}
|
TOKEN_LPAREN expression TOKEN_RPAREN
{
    $$ = $2;
}
|
TOKEN_PRINT expression
{
    $$ =  AstUnOp::make(PRINT, $2);
}
|
TOKEN_READINT
{
    $$ =  AstRead::make(true);
}
|
TOKEN_READSTRING
{
    $$ =  AstRead::make(false);
}
|
expression TOKEN_CONS expression
{
    $$ = AstBinOp::make(CONS, $1, $3);
}
|
TOKEN_HD expression
{
    $$ =  AstUnOp::make(HD, $2);
}
|
TOKEN_TL expression
{
    $$ =  AstUnOp::make(TL, $2);
}
|
TOKEN_NIL
{
    $$ =  AstNil::make();
}
|
TOKEN_ISNIL expression
{
    $$ =  AstUnOp::make(ISNIL, $2);
}
|
TOKEN_IDENTIFIER
{
    string lexeme = GET_LEXEME($1);
    $$ =  AstIdentifier::make(lexeme);
}
|
TOKEN_ERROR 
{
    // do not change the error rule
    string lexeme = GET_LEXEME($1);
    string error = "Lexing error: ";
    if(lexeme != "") error += lexeme;
    yyerror(error.c_str());
    YYERROR;
}



id_list:
TOKEN_IDENTIFIER {

    string lexeme = GET_LEXEME($1);
    AstIdentifier *id =  AstIdentifier::make(lexeme);
    $$ = AstIdentifierList::make(id);
}
| 
TOKEN_IDENTIFIER TOKEN_COMMA id_list {
    Expression *_i = $3;
    assert(_i->get_type() == AST_IDENTIFIER_LIST);
    AstIdentifierList* id_list = static_cast<AstIdentifierList*>(_i);
	
    string lexeme = GET_LEXEME($1);
    AstIdentifier *id =  AstIdentifier::make(lexeme);
    $$ = id_list->append_id(id);
}


expression_application: expression expression
{
    AstExpressionList *l = AstExpressionList::make($1);
    l = l->append_exp($2);
    $$ = l;
}
|
expression_application expression
{
    Expression* _l = $1;
    assert(_l->get_type() == AST_EXPRESSION_LIST);
    AstExpressionList* l = static_cast<AstExpressionList*>(_l);
    $$ = l->append_exp($2);
}