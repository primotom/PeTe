%{
#include <string>
#include "PQL.h"
#include "PQLConditionAssignmentParser.parser.hpp"

#define SAVE_TOKEN pqlcalval.string = new std::string(pqlcatext, pqlcaleng)
#define TOKEN(t) (pqlcalval.token = t)

extern "C" int pqlcawrap(){return 1;}
extern PetriEngine::PQL::AssignmentExpression* assignment;
extern int pqlcaparse();
%}
%option prefix="pqlca"
%option nounput

digit         [0-9]
letter        [a-zA-Z_]

%%

[ \t\n\r]					;
"and"						{return TOKEN(AND);}
"AND"						{return TOKEN(AND);}
"or"						{return TOKEN(OR);}
"OR"						{return TOKEN(OR);}
"not"						{return TOKEN(NOT);}
"NOT"						{return TOKEN(NOT);}
"true"						{return TOKEN(TRUE);}
"TRUE"						{return TOKEN(TRUE);}
"false"						{return TOKEN(FALSE);}
"FALSE"						{return TOKEN(FALSE);}
{letter}({letter}|{digit})*	{SAVE_TOKEN; return ID;}
{digit}+					{SAVE_TOKEN; return INT;}
"&&"						{return TOKEN(AND);}
"||"						{return TOKEN(OR);}
"!"							{return TOKEN(NOT);}
"=="						{return TOKEN(EQUAL);}
"!="						{return TOKEN(NEQUAL);}
"<"							{return TOKEN(LESS);}
"<="						{return TOKEN(LESSEQUAL);}
">"							{return TOKEN(GREATER);}
">="						{return TOKEN(GREATEREQUAL);}
"("							{return TOKEN(LPAREN);}
")"							{return TOKEN(RPAREN);}
"="							{return TOKEN(EQUAL);}
":="						{return TOKEN(ASSIGN);}
";"							{return TOKEN(SEMI);}
.							{printf("Unknown token %s!\n", pqlqtext); yyterminate();}

%%

namespace PetriEngine{ namespace PQL{
Condition* ParseAssignment(const std::string& queryString){
	assignment = NULL;

	//Load up input buffer in Flex
	YY_BUFFER_STATE buf = pqlca_scan_string(queryString.c_str());

	if(pqlcaparse() != 0)
		return NULL;

	//Delete the buffer
	pqlca_delete_buffer(buf);

	return assignment;
}
}}
