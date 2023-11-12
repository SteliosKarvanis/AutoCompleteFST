#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <vector>

struct Transition{
    Transition(char character) : character(character){};
    char character;
};

struct Node{
    bool valid = false;
    bool frozen = false;
    std::vector<Node*> next_nodes = std::vector<Node*>();
    std::vector<Node*> previous_nodes = std::vector<Node*>();
    std::vector<Transition*> forward_transitions = std::vector<Transition*>();
    std::vector<Transition*> backward_transitions = std::vector<Transition*>();
};

#endif