#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "nodes.h"

Scope* scope;
Sub* subs[26];

/**
 * Creates a new scope
 */
Scope* push_scope(Scope* previous_scope) {
    Scope* newScope = malloc(sizeof(Scope));
    int i;

    if (previous_scope == NULL) {
        newScope->previous_scope = NULL;

        for (i=0; i<26; i+=1) {
            newScope->symbols[i] = 0;
        }

    } else {
        newScope->previous_scope = previous_scope;
        // if a scope exists, copy all the values from the previous scope so we
        // dont change them; this language is function scoped (like JavaScript muhahaha)
        for (i=0; i<26; i+=1) {
            newScope->symbols[i] = previous_scope->symbols[i];
        }
    }

    return newScope;
}

/**
 * Pops the current one and returns the previous scope
 */
Scope* pop_scope(Scope* scope) {
    if (scope->previous_scope == NULL) {
        printf("Error! Popping last scope\n");
        exit(EXIT_FAILURE);
    } else {
        Scope* previous_scope = scope->previous_scope;
        free(scope);
        return previous_scope;
    }
}

int call_sub(Sub* sub, Node* callArguments, int debug, int indention) {
    // a function call creates a new scope and initializes the values
    scope = push_scope(scope);
    Node* subArg = sub->arguments;
    while (subArg != NULL
           && callArguments != NULL
           && callArguments->type != N_END_OF_ARG_LIST
           && subArg->type != N_END_OF_ARG_LIST) {
        int symbol = subArg->value;
        scope->symbols[symbol] = interpret(callArguments->left, debug, indention);
        callArguments = callArguments->middle;
        subArg = subArg->left;
    }

    interpret(sub->stmt, debug, indention);
    int result = scope->return_value;
    scope = pop_scope(scope);
    return result;
}

/**
 * Used to intialize the values like the scope to start interpreting
 */
int start_interpreter(Node* node, int debug, int indention) {
    scope = push_scope(NULL);
    return interpret(node, debug, indention);
}

/**
 * Interpret a node
 */
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
            return scope->symbols[node->value];

        case N_EQUAL:
            return interpret(node->left, debug, indention) == interpret(node->middle, debug, indention);

        case N_LESS:
            return interpret(node->left, debug, indention) < interpret(node->middle, debug, indention);

        case N_LESS_EQUAL:
            return interpret(node->left, debug, indention) <= interpret(node->middle, debug, indention);

        case N_BIGGER:
            return interpret(node->left, debug, indention) > interpret(node->middle, debug, indention);

        case N_BIGGER_EQUAL:
            return interpret(node->left, debug, indention) >= interpret(node->middle, debug, indention);

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
            scope->symbols[node->value] = interpret(node->left, debug, indention);
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
                scope->return_value = 0;
            } else {
                scope->return_value = interpret(node->left, debug, indention);
            }
            return 0;

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
            return call_sub(subs[node->value], node->left, debug, indention);

        case N_END_OF_PROGRAM:
            return 0;

        default:
            printf("Unknown node type!\n");
            return EXIT_FAILURE;
        }
    }
    return 0;
}


/**
 * Prints debug statements for a node
 */
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
