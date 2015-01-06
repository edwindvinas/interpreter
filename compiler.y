%{
#include <stdio.h>
#include <string.h>

void yyerror(const char *str) {
    fprintf(stderr, "error: %s\n", str);
}

int sym[26];

%}

%token T_INTEGER T_FLOAT T_VAR T_IDENTIFIER T_SEMICOLON T_EQUAL
%token T_BEGIN T_END T_LOOP T_FOR T_SUB T_IF T_THEN T_ELSE T_ASSIGN T_PRINT
%token T_PLUS T_MINUS T_DIVIDE T_MULTIPLY T_MODULO T_NOT T_COMMA T_RETURN
%token T_BINARY_OR T_BINARY_AND T_OPEN_BRACKET T_CLOSE_BRACKET T_UNARY_MINUS
%start stmt

%left T_BINARY_OR
%left T_BINARY_AND
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE T_MODULO
%left T_UNARY_MINUS

%%
stmt: /* empty */
    | T_LOOP stmt T_FOR expr
    | T_LOOP T_FOR expr stmt
    | stmt expr T_SEMICOLON
    | T_BEGIN stmt T_END
    | T_IF expr T_THEN stmt
    | T_SUB T_IDENTIFIER T_OPEN_BRACKET arguments T_CLOSE_BRACKET stmt
    | T_IF expr T_THEN stmt T_ELSE stmt

number: T_INTEGER
      | T_FLOAT

arguments: /* empty */
         | T_IDENTIFIER
         | T_IDENTIFIER T_COMMA arguments

expr: T_VAR T_IDENTIFIER T_ASSIGN expr {sym[$2] = $4; printf("%c", $1);}
    | T_PRINT expr {printf("%d\n", $2);}
    | number {$$ = $1;}
    | T_IDENTIFIER T_OPEN_BRACKET arguments T_CLOSE_BRACKET {printf("calling function");}
    | T_IDENTIFIER {$$ = sym[$1];}
    | expr T_PLUS expr {$$ = $1 + $3;}
    | expr T_MINUS expr {$$ = $1 - $3;}
    | expr T_MULTIPLY expr {$$ = $1 * $3;}
    | expr T_DIVIDE expr {$$ = $1 / $3;}
    | expr T_MODULO expr {$$ = $1 % $3;}
    | T_MINUS expr %prec T_UNARY_MINUS {$$ = -$2;}
    | T_OPEN_BRACKET expr T_CLOSE_BRACKET {$$ = $2;}
    | T_NOT expr {$$ = !$2;}
    | T_RETURN
    | T_RETURN expr


%%