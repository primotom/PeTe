%{
#include <stdio.h>
#include "PQL.h"
#include "Expressions.h"
using namespace PetriEngine::PQL;

Condition* query;
extern int pqlqlex();
void pqlqerror(const char *s) {printf("ERROR: %s\n", s);}
%}

%name-prefix "pqlq"

/* Possible data representation */
%union {
	PetriEngine::PQL::Expr* expr;
	PetriEngine::PQL::Condition* cond;
	std::string *string;
	int token;
}

/* Terminal type definition */
%token <string> ID INT
%token <token> LPAREN RPAREN
%token <token> AND OR NOT
%token <token> EQUAL NEQUAL LESS LESSEQUAL GREATER GREATEREQUAL
%token <token> PLUS MINUS MULTIPLY
%token <token> TRUE FALSE

/* Terminal associativity */
%left AND OR
%right NOT

/* Nonterminal type definition */
%type <expr> factor
%type <cond> logics logic

/* Operator precedence, more possibly coming */

%start query

%%

query	: logics					{ query = $1; }
		| error						{ yyerrok; }
		;

logics	: logics AND logic			{ $$ = new AndCondition($1, $3); }
		| logics OR logic			{ $$ = new OrCondition($1, $3); }
		| LPAREN logics RPAREN		{ $$ = $2; }
		| logic						{ $$ = $1; }
		;

logic	: ID EQUAL factor			{ $$ = new EqualCondition($1, $3); }
		| ID NEQUAL factor			{ $$ = new NotEqualCondition($1, $3); }
		| ID LESS factor			{ $$ = new LessThanCondition($1, $3); }
		| ID LESSEQUAL factor 		{ $$ = new LessThanOrEqualCondition($1, $3); }
		| ID GREATER factor			{ $$ = new GreaterThanCondition($1, $3); }
		| ID GREATEREQUAL factor	{ $$ = new GreaterThanOrEqualCondition($1, $3); }
		| ID						{ $$ = new VariableCondition(*$1, @1.first_column); delete $1; }
		| NOT ID					{ $$ = new NotCondition(new VariableCondition(*$2, @2.first_column)); delete $2; }
		;

factor	: INT						{ $$ = new LiteralExpr(atol($1->c_str())); delete $1; }
		| ID						{ $$ = new IdentifierExpr(*$1, @1.first_column); delete $1; }
		;


