#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <map>

struct Node{
    bool valid = false;
    std::map<char, Node*> next_nodes = std::map<char, Node*>();
};

bool compare_nodes(Node* actual_node, Node* new_node);

#endif