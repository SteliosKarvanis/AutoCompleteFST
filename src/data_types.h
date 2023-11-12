#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <vector>

struct Node{
    bool valid = false;
    bool frozen = false;
    std::vector<Node*> next_nodes = std::vector<Node*>();
    std::vector<Node*> previous_nodes = std::vector<Node*>();
    std::vector<char> forward_transitions = std::vector<char>();
    std::vector<char> backward_transitions = std::vector<char>();
};

#endif