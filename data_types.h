#ifndef DATA_TYPES_H
#define DATA_TYPES_H


#include <vector>

struct Transition{
    Transition(char character, int value=-1): character(character), value(value) {};
    char character;
    int value;
};

struct Node{
    bool valid = false;
    bool frozen = false;
    int depth = 0;
    std::vector<Node*> next_nodes = std::vector<Node*>();
    std::vector<Node*> previous_nodes = std::vector<Node*>();
    std::vector<Transition*> forward_transitions = std::vector<Transition*>();
    std::vector<Transition*> backward_transitions = std::vector<Transition*>();
};

#endif