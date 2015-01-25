#pragma once

#include "nodes.h"

int start_interpreter(Node* node, int debug, int indention);
int interpret(Node* node, int debug, int indention);
void debug_node(Node* node, int indention);
int call_sub(Sub* sub, Node* arguments, int debug, int indention);
Scope* push_scope(Scope* previous_scope);
Scope* pop_scope(Scope* scope);