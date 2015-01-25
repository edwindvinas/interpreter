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

