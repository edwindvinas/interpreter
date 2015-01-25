#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "nodes.h"

int symbols[26];
Sub* subs[26];


void debug_node(Node* node, int indention) {
    char *indentionSpaces = (char*)malloc(sizeof(char)*indention + 1);
    int i;
    for (i=0; i<indention; i++) {
        indentionSpaces[i] = ' ';
    }
    indentionSpaces[indention] = '\0';

    int type = -1;
    int value = -1;
    char* msg = "%sNode with type %s\n";
    if (node != NULL) {
        type = node->type;
        if (type == N_VARIABLE) {
            value = node->value + 'a';
            msg = "%sNode with type %s and value %c\n";
        } else {
            value = node->value;
        }

        if (type == N_VARIABLE || type == N_ASSIGNMENT || type == N_INT) {
            printf(msg, indentionSpaces,
                node_type_to_description(type), value);
        } else {
            printf(msg, indentionSpaces, node_type_to_description(type));
        }
    }
}

int call_sub(Sub* sub, Node* arguments) {

}

int interpret(Node* node, int debug, int indention) {
    if (debug == 1) {
        debug_node(node, indention);
    }
    indention++;

    if (node != NULL) {
        int i;
        int times;
        Sub* sub;

        switch (node->type) {

        case N_STMT:
            interpret(node->left, debug, indention);
            if (node->middle != NULL) {
                interpret(node->middle, debug, indention);
            }
            break;

        case N_IF:
            if (interpret(node->left, debug, indention)) {
                return interpret(node->middle, debug, indention);
            }
            return 0;

        case N_IF_ELSE:
            if (interpret(node->left, debug, indention)) {
                return interpret(node->middle, debug, indention);
            } else {
                return interpret(node->right, debug, indention);
            }
            return 0;

        case N_INT:
            return node->value;

        case N_VARIABLE:
            return symbols[node->value];

        case N_EQUAL:
            return interpret(node->left, debug, indention) == interpret(node->middle, debug, indention);

        case N_ADDITION:
            return interpret(node->left, debug, indention) + interpret(node->middle, debug, indention);

        case N_SUBTRACTION:
            return interpret(node->left, debug, indention) - interpret(node->middle, debug, indention);

        case N_MULTIPLY:
            return interpret(node->left, debug, indention) * interpret(node->middle, debug, indention);

        case N_DIVIDE:
            return interpret(node->left, debug, indention) / interpret(node->middle, debug, indention);

        case N_MODULO:
            return interpret(node->left, debug, indention) % interpret(node->middle, debug, indention);

        case N_BINARY_OR:
            return interpret(node->left, debug, indention) | interpret(node->middle, debug, indention);

        case N_BINARY_AND:
            return interpret(node->left, debug, indention) & interpret(node->middle, debug, indention);

        case N_ASSIGNMENT:
            symbols[node->value] = interpret(node->left, debug, indention);
            return 0;

        case N_NOT:
            return ! interpret(node->left, debug, indention);

        case N_MINUS:
            return -1 * interpret(node->left, debug, indention);

        case N_PRINT:
            printf("%d\n", interpret(node->left, debug, indention));
            return 0;

        case N_RETURN:
            if (node->left == NULL) {
                return 0;
            } else {
                return interpret(node->left, debug, indention);
            }

        case N_LOOP_BLOCK_FOR:
            times = interpret(node->middle, debug, indention);
            for (i=0; i<times; i+=1) {
                interpret(node->left, debug, indention);
            }
            return 0;

        case N_LOOP_FOR_BLOCK:
            times = interpret(node->left, debug, indention);
            for (i=0; i<times; i+=1) {
                interpret(node->middle, debug, indention);
            }
            return 0;

        case N_SUB:
            sub = malloc(sizeof(Sub));
            sub->stmt = node->middle;
            sub->arguments = node->left;
            subs[node->value] = sub;
            return 0;

        case N_CALL:
            return call_sub(subs[node->value], node->left);

        case N_END_OF_PROGRAM:
            return 0;

        default:
            printf("Unknown node type!\n");
            return EXIT_FAILURE;
        }
    }
    return 0;
}