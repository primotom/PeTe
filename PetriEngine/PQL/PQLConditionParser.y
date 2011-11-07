%{
#include <stdio.h>
#include "PQL.h"
#include "Expressions.h"
using namespace PetriEngine::PQL;

Condition* query;
extern int pqlclex();
void pqlcerror(const char *s) {printf("ERROR: %s\n", s);}
%}

%name-prefix "pqlc"

/* Data representation */
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

/* Terminal associativity */
%left AND OR
%right NOT

/* Nonterminal type definition */
%type <expr> expr
%type <cond> logic compare

%start query

%%

query	: logic					{ query = $1; }
		| error					{ yyerrok; }
		;

logic	: logic AND logic		{ $$ = new AndCondition($1, $3); }
		| logic OR logic		{ $$ = new OrCondition($1, $3); }
		| LPAREN logic RPAREN	{ $$ = $2; }
		| ID					{ $$ = new VariableCondition(*$1, @1.first_column); delete $1; }
		| NOT ID				{ $$ = new NotCondition(new VariableCondition(*$1, @1.first_column)); 
								  delete $1; }
		| compare				{ $$ = $1; }
		;

compare	: expr EQUAL expr			{ $$ = new EqualCondition($1, $3); }
		| expr NEQUAL expr			{ $$ = new NotEqualCondition($1, $3); }
		| expr LESS expr			{ $$ = new LessThanCondition($1, $3); }
		| expr LESSEQUAL expr 		{ $$ = new LessThanOrEqualCondition($1, $3); }
		| expr GREATER expr			{ $$ = new GreaterThanCondition($1, $3); }
		| expr GREATEREQUAL expr	{ $$ = new GreaterThanOrEqualCondition($1, $3); }
		;

expr	: INT			{ $$ = new LiteralExpr(atol($1->c_str())); delete $1; }
		| ID			{ $$ = new IdentifierExpr(*$1, @1.first_column); delete $1; }
		;
