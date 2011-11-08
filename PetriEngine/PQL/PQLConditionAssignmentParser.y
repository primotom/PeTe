%{
#include <stdio.h>
#include "PQL.h"
#include "Expressions.h"
using namespace PetriEngine::PQL;

AssignmentExpression* conditionAssignment;
extern int pqlcalex();
void pqlcaerror(const char *s) {printf("ERROR: %s\n", s);}
%}

%name-prefix "pqlca"

/* Data representation */
%union {
	PetriEngine::PQL::AssignmentExpression* assign;
	PetriEngine::PQL::Expr* expr;
	PetriEngine::PQL::Condition* cond;
	std::string *string;
	int token;
}

/* Terminal type definition */
%token <string> ID INT
%token <token> LPAREN RPAREN ASSIGN SEMI
%token <token> AND OR NOT
%token <token> TRUE FALSE
%token <token> EQUAL NEQUAL LESS LESSEQUAL GREATER GREATEREQUAL

/* Terminal associativity */
%left AND OR
%right NOT

/* Nonterminal type definition */
%type <expr> expr
%type <cond> logic compare
%type <assign> assignment

%start root

%%

root	: assignment			{ conditionAssignment = $1; }
		| error					{ yyerrok; }
		;

assignment	: ID ASSIGN logic SEMI assignment 	{ ((AssignmentExpression*)$5)->prepend(*$1, $3); delete $1;
												  $$ = ((AssignmentExpression*)$5)}
			| ID ASSIGN logic SEMI				{ AssignmentExpression* a = new AssignmentExpression();
												  a->prepend(*$1, $3); delete $1;
												  $$ = a;}
			| ID ASSIGN logic					{ AssignmentExpression* a = new AssignmentExpression();
												  a->prepend(*$1, $3); delete $1;
												  $$ = a;}
			;

logic	: logic AND logic		{ $$ = new AndCondition($1, $3); }
		| logic OR logic		{ $$ = new OrCondition($1, $3); }
		| LPAREN logic RPAREN	{ $$ = $2; }
		| ID					{ $$ = new VariableCondition(*$1, @1.first_column); delete $1; }
		| NOT ID				{ $$ = new NotCondition(new VariableCondition(*$2, @1.first_column));
								  delete $2; }
		| TRUE					{ $$ = new BooleanLiteral(true); }
		| FALSE					{ $$ = new BooleanLiteral(false); }
		| compare				{ $$ = $1; }
		;

compare	: expr EQUAL expr			{ $$ = new EqualCondition($1, $3); }
		| expr NEQUAL expr			{ $$ = new NotEqualCondition($1, $3); }
		| expr LESS expr			{ $$ = new LessThanCondition($1, $3); }
		| expr LESSEQUAL expr 		{ $$ = new LessThanOrEqualCondition($1, $3); }
		| expr GREATER expr			{ $$ = new GreaterThanCondition($1, $3); }
		| expr GREATEREQUAL expr	{ $$ = new GreaterThanOrEqualCondition($1, $3); }
		;

expr	: INT			{ $$ = new IntegerLiteralExpr(atol($1->c_str())); delete $1; }
		| ID			{ $$ = new IdentifierExpr(*$1, @1.first_column); delete $1; }
		;
