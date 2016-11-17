#include <stdlib.h>

#include "nodes.h"


Node* create_node(NodeType type, Node* left, Node* middle, Node* right, int value) {
    Node* node = malloc(sizeof(Node));
    node->type = type;
    node->left = left;
    node->middle = middle;
    node->right = right;
    node->value = value;
    return node;
}

char* node_type_to_description(NodeType type) {
    switch (type) {
    case N_EMPTY:
        return "empty";

    case N_ASSIGNMENT:
        return "assignment";

    case N_INT:
        return "int";

    case N_VARIABLE:
        return "variable";

    case N_ADDITION:
        return "addition";

    case N_SUBTRACTION:
        return "subtraction";

    case N_MULTIPLY:
        return "multiply";

    case N_DIVIDE:
        return "divide";

    case N_MODULO:
        return "modulo";

    case N_LOOP_BLOCK_FOR:
        return "loop block for";

    case N_LOOP_FOR_BLOCK:
        return "loop for block";

    case N_IF:
        return "if";

    case N_IF_ELSE:
        return "if else";

    case N_SUB:
        return "sub";

    case N_CALL:
        return "call";

    case N_PRINT:
        return "print";

    case N_STMT:
        return "stmt";

    case N_EQUAL:
        return "equal";

    case N_NOT:
        return "not";

    case N_MINUS:
        return "minus";

    case N_RETURN:
        return "return";

    case N_ARGUMENT:
        return "argument";

    case N_BINARY_OR:
        return "binary or";

    case N_BINARY_AND:
        return "binary and";

    case N_LESS:
        return "less";

    case N_LESS_EQUAL:
        return "less equal";

    case N_BIGGER:
        return "greater";

    case N_BIGGER_EQUAL:
        return "greater equal";

    case N_END_OF_PROGRAM:
        return "end of program";

    case N_END_OF_ARG_LIST:
        return "end of argument list";

    default:
        return "unknown";
    }

}
