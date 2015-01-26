%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodes.h"
#include "interpreter.h"
#include "cmd.h"

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
%type <node> argument_list
%type <node> call_arguments
%type <node> call_argument_list
%type <integer> T_INTEGER
%type <integer> T_IDENTIFIER

%token T_VAR T_BEGIN T_END T_LOOP T_FOR T_SUB T_IF T_THEN T_ELSE T_PRINT T_RETURN
%token T_BIGGER T_LESS T_LESS_EQUAL T_BIGGER_EQUAL T_ASSIGN T_SEMICOLON T_EQUAL T_PLUS T_MINUS T_DIVIDE T_MULTIPLY T_MODULO T_NOT T_COMMA T_BINARY_OR T_BINARY_AND T_OPEN_BRACKET T_CLOSE_BRACKET T_UNARY_MINUS
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

stmts: /* empty */ {$$ = create_node(N_END_OF_PROGRAM, NULL, NULL, NULL, 0);}
     | stmt stmts {$$ = create_node(N_STMT, $1, $2, NULL, 0);}

stmt: T_LOOP stmt T_FOR expr {$$ = create_node(N_LOOP_BLOCK_FOR, $2, $4, NULL, 0);}
    | T_LOOP T_FOR expr stmt {$$ = create_node(N_LOOP_FOR_BLOCK, $3, $4, NULL, 0);}
    | expr T_SEMICOLON
    | T_BEGIN stmts T_END {$$ = $2;}
    | T_IF expr T_THEN stmt {$$ = create_node(N_IF, $2, $4, NULL, 0);}
    | T_SUB T_IDENTIFIER T_OPEN_BRACKET arguments T_CLOSE_BRACKET stmt {$$ = create_node(N_SUB, $4, $6, NULL, $2);}
    | T_IF expr T_THEN stmt T_ELSE stmt {$$ = create_node(N_IF_ELSE, $2, $4, $6, 0);}

arguments: /* empty */ {$$ = create_node(N_END_OF_ARG_LIST, NULL, NULL, NULL, 0);}
         | argument_list

argument_list: T_IDENTIFIER {$$ = create_node(N_ARGUMENT, NULL, NULL, NULL, $1);}
             | T_IDENTIFIER T_COMMA argument_list {$$ = create_node(N_ARGUMENT, $3, NULL, NULL, $1);}

call_arguments: /* empty */ {$$ = create_node(N_END_OF_ARG_LIST, NULL, NULL, NULL, 0);}
         | call_argument_list

call_argument_list: expr {$$ = create_node(N_ARGUMENT, $1, NULL, NULL, 0);}
                  | expr T_COMMA call_argument_list {$$ = create_node(N_ARGUMENT, $1, $3, NULL, 0);}

expr:
    T_VAR T_IDENTIFIER T_ASSIGN expr {$$ = create_node(N_ASSIGNMENT, $4, NULL, NULL, $2);}
    | T_PRINT expr {$$ = create_node(N_PRINT, $2, NULL, NULL, 0);}
    | T_INTEGER {$$ = create_node(N_INT, NULL, NULL, NULL, $1);}
    | expr T_EQUAL expr {$$ = create_node(N_EQUAL, $1, $3, NULL, 0);}
    | T_IDENTIFIER T_OPEN_BRACKET call_arguments T_CLOSE_BRACKET {$$ = create_node(N_CALL, $3, NULL, NULL, $1);}
    | T_IDENTIFIER {$$ = create_node(N_VARIABLE, NULL, NULL, NULL, $1);}
    | expr T_BINARY_OR expr {$$ = create_node(N_BINARY_OR, $1, $3, NULL, 0);}
    | expr T_BINARY_AND expr {$$ = create_node(N_BINARY_AND, $1, $3, NULL, 0);}
    | expr T_PLUS expr {$$ = create_node(N_ADDITION, $1, $3, NULL, 0);}
    | expr T_MINUS expr {$$ = create_node(N_SUBTRACTION, $1, $3, NULL, 0);}
    | expr T_MULTIPLY expr {$$ = create_node(N_MULTIPLY, $1, $3, NULL, 0);}
    | expr T_DIVIDE expr {$$ = create_node(N_DIVIDE, $1, $3, NULL, 0);}
    | expr T_MODULO expr {$$ = create_node(N_MODULO, $1, $3, NULL, 0);}
    | expr T_LESS expr {$$ = create_node(N_LESS, $1, $3, NULL, 0);}
    | expr T_LESS_EQUAL expr {$$ = create_node(N_LESS_EQUAL, $1, $3, NULL, 0);}
    | expr T_BIGGER expr {$$ = create_node(N_BIGGER, $1, $3, NULL, 0);}
    | expr T_BIGGER_EQUAL expr {$$ = create_node(N_BIGGER_EQUAL, $1, $3, NULL, 0);}
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
    Options* options = parse_cmd(argc, argv);

    yyin = fopen(options->file, "r");

    if (yyin == NULL) {
        printf("Could not read file!\n");
        return EXIT_FAILURE;
    }

    yyparse();
    start_interpreter(root, options->debug, 0);
    return EXIT_SUCCESS;
}

