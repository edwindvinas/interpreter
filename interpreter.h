#pragma once

#include "nodes.h"

int interpret(Node* node, int debug, int indention);
void debug_node(Node* node, int indention);
int call_sub(Sub* sub, Node* arguments);