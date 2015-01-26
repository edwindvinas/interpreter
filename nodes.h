#pragma once

typedef enum {
    N_EMPTY,
    N_ASSIGNMENT,
    N_INT,
    N_VARIABLE,
    N_ADDITION,
    N_SUBTRACTION,
    N_MULTIPLY,
    N_DIVIDE,
    N_MODULO,
    N_LOOP_BLOCK_FOR,
    N_LOOP_FOR_BLOCK,
    N_IF,
    N_IF_ELSE,
    N_SUB,
    N_CALL,
    N_PRINT,
    N_STMT,
    N_EQUAL,
    N_NOT,
    N_MINUS,
    N_RETURN,
    N_ARGUMENT,
    N_BINARY_OR,
    N_BINARY_AND,
    N_LESS,
    N_LESS_EQUAL,
    N_BIGGER,
    N_BIGGER_EQUAL,
    N_END_OF_PROGRAM,
    N_END_OF_ARG_LIST
} NodeType;

typedef struct node {
    NodeType type;
    int value;
    struct node* left;
    struct node* middle;
    struct node* right;
} Node;

typedef struct sub {
    Node* stmt;
    Node* arguments;
} Sub;

typedef struct scope {
    int return_value;
    int symbols[26];
    struct scope* previous_scope;
} Scope;

Node* create_node(NodeType type, Node* left, Node* middle, Node* right, int value);
char* node_type_to_description(NodeType type);