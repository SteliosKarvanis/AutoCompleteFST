#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <map>

struct Node{
    bool valid = false;
    std::map<char, Node*> next_nodes = std::map<char, Node*>();
};

#endif