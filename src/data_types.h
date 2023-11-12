#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <vector>
#include <memory>

#define shp std::shared_ptr
#define mshp std::make_shared

struct Transition{
    Transition(char character) : character(character){};
    char character;
};

struct Node{
    bool valid = false;
    bool frozen = false;
    std::vector<shp<Node>> next_nodes = std::vector<shp<Node>>();
    std::vector<shp<Node>> previous_nodes = std::vector<shp<Node>>();
    std::vector<shp<Transition>> forward_transitions = std::vector<shp<Transition>>();
    std::vector<shp<Transition>> backward_transitions = std::vector<shp<Transition>>();
};

#endif