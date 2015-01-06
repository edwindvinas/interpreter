%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE* yyin;
extern int* yylineno;

void yyerror(const char *str) {
    fprintf(stderr, "line %d: %s\n", yylineno, str);
}

int symbols[26];
int functions[26];



%}
%token T_VAR T_BEGIN T_END T_LOOP T_FOR T_SUB T_IF T_THEN T_ELSE T_PRINT T_RETURN
%token T_ASSIGN T_SEMICOLON T_EQUAL T_PLUS T_MINUS T_DIVIDE T_MULTIPLY T_MODULO T_NOT T_COMMA T_BINARY_OR T_BINARY_AND T_OPEN_BRACKET T_CLOSE_BRACKET T_UNARY_MINUS
%token T_IDENTIFIER
%token T_INTEGER
%start stmt

%left T_BINARY_OR
%left T_BINARY_AND
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE T_MODULO
%left T_UNARY_MINUS

%%
stmt: /* empty */
    | T_LOOP stmt T_FOR expr
    | T_LOOP T_FOR number stmt
    | stmt expr T_SEMICOLON
    | T_BEGIN stmt T_END
    | T_IF expr T_THEN stmt {printf("if\n");}
    | T_SUB T_IDENTIFIER T_OPEN_BRACKET arguments T_CLOSE_BRACKET stmt
    | T_IF expr T_THEN stmt T_ELSE stmt {printf("if\n");}

number: T_INTEGER

arguments: /* empty */
         | T_IDENTIFIER
         | arguments T_COMMA T_IDENTIFIER

expr: T_VAR T_IDENTIFIER T_ASSIGN expr {symbols[$2] = $4;}
    | T_PRINT expr {printf("%d\n", $2);}
    | number {$$ = $1;}
    | expr T_EQUAL expr {$$ = $1 == $2;}
    | T_IDENTIFIER T_OPEN_BRACKET arguments T_CLOSE_BRACKET {printf("calling function");}
    | T_IDENTIFIER {$$ = symbols[$1];}
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

void scan_string(const char* str) {
    yy_switch_to_buffer(yy_scan_string(str));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please provide a path to a program as the first parameter!\n");
        return EXIT_FAILURE;
    } else {
        yyin = fopen(argv[1], "r");

        if (yyin == NULL) {
            printf("Could not read file!\n");
            return EXIT_FAILURE;
        }

        yyparse();
        return 0;
    }
}

