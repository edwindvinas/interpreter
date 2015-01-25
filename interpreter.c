#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "nodes.h"

int symbols[26];
int subs[26];

void debug_node(Node* node, int indention) {
    char *indentionSpaces = (char*)malloc(sizeof(char)*indention + 1);
    int i;
    for (i=0; i<indention; i++) {
        indentionSpaces[i] = ' ';
    }
    indentionSpaces[indention] = '\0';

    int type = -1;
    int value = -1;
    if (node != NULL) {
        type = node->type;
        value = node->value;
    }
    printf("%sNode with type %s and value %d\n", indentionSpaces,
        node_type_to_description(type), value);
}

int interpret(Node* node, int debug, int indention) {
    if (debug == 1) {
        debug_node(node, indention);
    }
    indention++;

    if (node != NULL) {
        int i;

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
            symbols[node->value] = interpret(node->middle, debug, indention);
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
            for (i=0; i<interpret(node->middle, debug, indention); i++) {
                interpret(node->left, debug, indention);
            }
            return 0;

        case N_LOOP_FOR_BLOCK:
            for (i=0; i<interpret(node->left, debug, indention); i++) {
                interpret(node->middle, debug, indention);
            }
            return 0;

        case N_SUB:
            printf("Not yet implementd\n");
            return 0;

        case N_ARGUMENT:
            printf("Not yet implementd\n");
            return 0;

        case N_CALL:
            printf("Not yet implementd\n");
            return 0;

        default:
            printf("Unknown node type!\n");
            return EXIT_FAILURE;
        }
    }
    return 0;
}