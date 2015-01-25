#pragma once

#include "nodes.h"

int interpret(Node* node, int debug, int indention);
void debug_node(Node* node, int indention);
int* create_symbol_table(int length);