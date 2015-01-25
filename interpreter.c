#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"

int symbols[26];
int subs[26];

int interpret(Node* node) {
    if (node != NULL) {
        int i;

        switch (node->type) {

        case N_STMT:
            interpret(node->left);
            if (node->middle != NULL) {
                interpret(node->middle);
            }
            break;

        case N_IF:
            if (interpret(node->left)) {
                return interpret(node->middle);
            }
            return 0;

        case N_IF_ELSE:
            if (interpret(node->left)) {
                return interpret(node->middle);
            } else {
                return interpret(node->right);
            }
            return 0;

        case N_INT:
            return node->value;

        case N_VARIABLE:
            return symbols[node->value];

        case N_EQUAL:
            return interpret(node->left) == interpret(node->middle);

        case N_ADDITION:
            return interpret(node->left) + interpret(node->middle);

        case N_SUBTRACTION:
            return interpret(node->left) - interpret(node->middle);

        case N_MULTIPLY:
            return interpret(node->left) * interpret(node->middle);

        case N_DIVIDE:
            return interpret(node->left) / interpret(node->middle);

        case N_MODULO:
            return interpret(node->left) % interpret(node->middle);

        case N_BINARY_OR:
            return interpret(node->left) | interpret(node->middle);

        case N_BINARY_AND:
            return interpret(node->left) & interpret(node->middle);

        case N_ASSIGNMENT:
            symbols[(int) node->value] = interpret(node->middle);
            return 0;

        case N_NOT:
            return ! interpret(node->left);

        case N_MINUS:
            return -1 * interpret(node->left);

        case N_PRINT:
            printf("%d\n", interpret(node->left));
            return 0;

        case N_RETURN:
            if (node->left == NULL) {
                return 0;
            } else {
                return interpret(node->left);
            }

        case N_LOOP_BLOCK_FOR:
            for (i=0; i<interpret(node->middle); i++) {
                interpret(node->left);
            }
            return 0;

        case N_LOOP_FOR_BLOCK:
            for (i=0; i<interpret(node->left); i++) {
                interpret(node->middle);
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