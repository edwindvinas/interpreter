%option noyywrap

%{
#include "nodes.h"
#include "y.tab.h"
%}

%%
var return T_VAR;
begin return T_BEGIN;
end return T_END;
for return T_FOR;
loop return T_LOOP;
sub return T_SUB;
if return T_IF;
then return T_THEN;
else return T_ELSE;
not return T_NOT;
return return T_RETURN;
print return T_PRINT;

[a-zA-Z][0-9a-zA-Z_]* {yylval.integer = *yytext - 'a'; return T_IDENTIFIER;}
[0123456789]+ {yylval.integer = atoi(yytext); return T_INTEGER;}
; return T_SEMICOLON;
, return T_COMMA;
\< return T_LESS;
\> return T_BIGGER;
\<= return T_LESS_EQUAL;
\>= return T_BIGGER_EQUAL;
= return T_EQUAL;
:= return T_ASSIGN;
\* return T_MULTIPLY;
\/ return T_DIVIDE;
\+ return T_PLUS;
- return T_MINUS;
% return T_MODULO;
\| return T_BINARY_OR;
& return T_BINARY_AND;
\( return T_OPEN_BRACKET;
\) return T_CLOSE_BRACKET;
[ \t\r]+ /* ignore whitespace */;
\n {yylineno++;}
. printf("Encountered invalid character");
%%
