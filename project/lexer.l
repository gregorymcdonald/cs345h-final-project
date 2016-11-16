/* Greg McDonald (grm695@utexas.edu) */
%{
/* TODO: Consider reordering rules */
/* TODO: Comments in L */

#include "parser-defs.h"
#include "parser.tab.h"

/*
 * You may want to use this string to assemble the lexeme
 * of string constants by adding the current matched character from yytext to it.
 * You can do this as buffer += yytext;
 */
string buffer;


/*
 * You may want to track the comment nesting depth with this variable.
 */
int comment_depth = 0;

%}


%option noyywrap

WHITESPACE [ \t\f\r\v]+
NEWLINE [\n]

DIGIT [0-9]
ALPHA_CHARACTER ([a-z]|[A-Z]|"_")
NUMERIC_CHARACTER {DIGIT}
ALPHANUMERIC_CHARACTER ({ALPHA_CHARACTER}|{NUMERIC_CHARACTER})
  
 
%x STRING
%x COMMENT 

%%



{WHITESPACE} {
    /* Do nothing */
}

{NEWLINE} {
    /* Do nothing, but increment line numbers */
    curr_lineno++;
}



{
	/* 
	 * My Rules - in order of listing in assignment
	 * XXX: change all rules that start with "XXX"
	 */

	/*
("\""(.|{NEWLINE})*"\"") {
    SET_LEXEME(yytext);
    return TOKEN_STRING;
}
*/

}

"(*" {
	comment_depth++;
	BEGIN(COMMENT);
}

<COMMENT>.|{NEWLINE} {
	// Do nothing for any comment characters, except open/close comment
}

<COMMENT>"(*" {
	comment_depth++;
}

<COMMENT>"*)" {
	comment_depth--;
	if(comment_depth == 0){
		BEGIN(INITIAL);
	}
}

"\"" {
	BEGIN(STRING);
}

<STRING>"\"" {
	BEGIN(INITIAL);
	SET_LEXEME(buffer);
	buffer.clear();
	return TOKEN_STRING;
}

<STRING>.|{NEWLINE} {
    buffer += yytext;
}


(?i:"readString") {
	return TOKEN_READSTRING;
}

(?i:"readInt") {
	return TOKEN_READINT;
}

(?i:"print") {
	return TOKEN_PRINT;
}

(?i:"isNil") {
	return TOKEN_ISNIL;
}

"!" {
	return TOKEN_HD;
}

"#" {
	return TOKEN_TL;
}

"@" {
	return TOKEN_CONS;
}

(?i:"Nil") {
	return TOKEN_NIL;
}

"." {
	return TOKEN_DOT;
}

(?i:"with") {
	return TOKEN_WITH;
}

(?i:"let") {
	return TOKEN_LET;
}

"+" {
	return TOKEN_PLUS;
}

"-" {
	return TOKEN_MINUS;
}

"*" {
	return TOKEN_TIMES;
}

"/" {
	return TOKEN_DIVIDE;
}

({DIGIT}+) {
	SET_LEXEME(yytext)
	return TOKEN_INT;
}

"(" {
	return TOKEN_LPAREN;
}

")" {
	return TOKEN_RPAREN;
}

"&" {
	return TOKEN_AND;
}

"|" {
	return TOKEN_OR;
}

"=" {
	return TOKEN_EQ;
}

"<>" {
	return TOKEN_NEQ;
}

">" {
	return TOKEN_GT;
}

">=" {
	return TOKEN_GEQ;
}

"<" {
	return TOKEN_LT;
}

"<=" {
	return TOKEN_LEQ;
}

(?i:"if") {
	return TOKEN_IF;
}

(?i:"then") {
	return TOKEN_THEN;
}

(?i:"else") {
	return TOKEN_ELSE;
}

(?i:"lambda") {
	return TOKEN_LAMBDA;
}

(?i:"fun") {
	return TOKEN_FUN;
}

"," {
    return TOKEN_COMMA;
}

(?i:"in") {
	return TOKEN_IN;
}

({ALPHA_CHARACTER}{ALPHANUMERIC_CHARACTER}*) {
	SET_LEXEME(yytext);
	return TOKEN_IDENTIFIER;
}

. {
    /* Leave this rule at the end of our lexer to "eat up" all illegal strings */
    SET_LEXEME("Unexpected character in input");
    return TOKEN_ERROR;
}



