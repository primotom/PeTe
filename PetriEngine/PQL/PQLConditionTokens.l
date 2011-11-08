%{
#include <string>
#include "PQL.h"
#include "PQLConditionParser.parser.hpp"

#define SAVE_TOKEN pqlclval.string = new std::string(pqlctext, pqlcleng)
#define TOKEN(t) (pqlclval.token = t)

extern "C" int pqlcwrap(){return 1;}
extern PetriEngine::PQL::Condition* conditionQuery;
extern int pqlcparse();
%}
%option prefix="pqlc"
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
.							{printf("Unknown token %s!\n", pqlctext); yyterminate();}

%%

namespace PetriEngine{ namespace PQL{
Condition* ParseCondition(const std::string& queryString){
	conditionQuery = NULL;

	//Load up input buffer in Flex
	YY_BUFFER_STATE buf = pqlc_scan_string(queryString.c_str());

	if(pqlcparse() != 0)
		return NULL;

	//Delete the buffer
	pqlc_delete_buffer(buf);

	return conditionQuery;
}
}}
