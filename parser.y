%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodes.h"
#include "interpreter.h"

extern FILE* yyin;
extern int* yylineno;

void yyerror(const char *str) {
    fprintf(stderr, "line %d: %s\n", yylineno, str);
}

Node* root;

%}
%union {
    int integer;
    Node* node;
}

%type <node> root
%type <node> stmts
%type <node> stmt
%type <node> expr
%type <node> arguments
%type <integer> T_INTEGER
%type <integer> T_IDENTIFIER

%token T_VAR T_BEGIN T_END T_LOOP T_FOR T_SUB T_IF T_THEN T_ELSE T_PRINT T_RETURN
%token T_ASSIGN T_SEMICOLON T_EQUAL T_PLUS T_MINUS T_DIVIDE T_MULTIPLY T_MODULO T_NOT T_COMMA T_BINARY_OR T_BINARY_AND T_OPEN_BRACKET T_CLOSE_BRACKET T_UNARY_MINUS
%token T_IDENTIFIER
%token T_INTEGER
%start root

%left T_BINARY_OR
%left T_BINARY_AND
%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE T_MODULO
%left T_UNARY_MINUS

%%
root: stmts {$$ = $1; root = $$; }

stmts: /* empty */ {$$ = NULL;}
     | stmt stmts {$$ = create_node(N_STMT, $1, $2, NULL, 0);}

stmt: /* empty */ {$$ = NULL;}
    | T_LOOP stmts T_FOR expr {$$ = create_node(N_LOOP_BLOCK_FOR, $2, $4, NULL, 0);}
    | T_LOOP T_FOR expr stmts {$$ = create_node(N_LOOP_FOR_BLOCK, $3, $4, NULL, 0);}
    | stmt expr T_SEMICOLON
    | T_BEGIN stmts T_END {$$ = $2;}
    | T_IF expr T_THEN stmt {$$ = create_node(N_IF, $2, $4, NULL, 0);}
    | T_SUB T_IDENTIFIER T_OPEN_BRACKET arguments T_CLOSE_BRACKET stmts {$$ = create_node(N_SUB, $4, $6, NULL, $2);}
    | T_IF expr T_THEN stmts T_ELSE stmts {$$ = create_node(N_IF_ELSE, $2, $4, $6, 0);}

arguments: T_IDENTIFIER {$$ = create_node(N_ARGUMENT, NULL, NULL, NULL, $1);}
         | arguments T_COMMA T_IDENTIFIER {$$ = create_node(N_ARGUMENT, $1, NULL, NULL, $3);}

expr:
    T_VAR T_IDENTIFIER T_ASSIGN expr {$$ = create_node(N_ASSIGNMENT, $4, NULL, NULL, $2);}
    | T_PRINT expr {$$ = create_node(N_PRINT, $2, NULL, NULL, 0);}
    | T_INTEGER {$$ = create_node(N_INT, NULL, NULL, NULL, $1);}
    | expr T_EQUAL expr {$$ = create_node(N_EQUAL, $1, $3, NULL, 0);}
    | T_IDENTIFIER T_OPEN_BRACKET arguments T_CLOSE_BRACKET {printf("calling function");}
    | T_IDENTIFIER {$$ = create_node(N_VARIABLE, NULL, NULL, NULL, $1);}
    | expr T_BINARY_OR expr {$$ = create_node(N_BINARY_OR, $1, $3, NULL, 0);}
    | expr T_BINARY_AND expr {$$ = create_node(N_BINARY_AND, $1, $3, NULL, 0);}
    | expr T_PLUS expr {$$ = create_node(N_ADDITION, $1, $3, NULL, 0);}
    | expr T_MINUS expr {$$ = create_node(N_SUBTRACTION, $1, $3, NULL, 0);}
    | expr T_MULTIPLY expr {$$ = create_node(N_MULTIPLY, $1, $3, NULL, 0);}
    | expr T_DIVIDE expr {$$ = create_node(N_DIVIDE, $1, $3, NULL, 0);}
    | expr T_MODULO expr {$$ = create_node(N_MODULO, $1, $3, NULL, 0);}
    | T_MINUS expr %prec T_UNARY_MINUS {$$ = create_node(N_MINUS, $2, NULL, NULL, 0);}
    | T_OPEN_BRACKET expr T_CLOSE_BRACKET {$$ = $2;}
    | T_NOT expr {$$ = create_node(N_NOT, $2, NULL, NULL, 0);}
    | T_RETURN {$$ = create_node(N_RETURN, NULL, NULL, NULL, 0);}
    | T_RETURN expr {$$ = create_node(N_RETURN, $2, NULL, NULL, 0);}


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
        interpret(root, 1, 0);
        return EXIT_SUCCESS;
    }
}

