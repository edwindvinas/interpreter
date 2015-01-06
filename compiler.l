%option noyywrap

%{
#include "y.tab.h"
%}

%%
"var" return T_VAR;
"begin" return T_BEGIN;
"end" return T_END;
"for" return T_FOR;
"loop" return T_LOOP;
"sub" return T_SUB;
"if" return T_IF;
"then" return T_THEN;
"else" return T_ELSE;
"not" return T_NOT;
"return" return T_RETURN;

[0123456789]+ return T_INTEGER;
[0123456789]+\.[0123456789]+ return T_FLOAT;
[a-zA-Z][0-9a-zA-Z_]* return T_IDENTIFIER;
";" return T_SEMICOLON;
"," return T_COMMA;
"=" return T_EQUAL;
":=" return T_ASSIGN;
"*" return T_MULTIPLY;
"/" return T_DIVIDE;
"+" return T_PLUS;
"-" return T_MINUS;
"%" return T_MODULO;
"|" return T_BINARY_OR;
"&" return T_BINARY_AND;
"(" return T_OPEN_BRACKET;
")" return T_CLOSE_BRACKET;

. printf("Encountered invalid character");
%%