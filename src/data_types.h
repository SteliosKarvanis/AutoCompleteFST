#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <map>
#include <set>

struct Node{
    bool valid = false;
    bool frozen = false;
    std::map<char, Node*> next_nodes;
    std::set<Node*> previous_nodes;
};

#endif