#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <map>

struct Node{
    bool valid = false;
    std::map<char, Node*> next_nodes = std::map<char, Node*>();
    bool equals(Node* other_node){
        if((this->valid != other_node->valid))
            return false;
        if(this->next_nodes != other_node->next_nodes)
            return false;
        return true;
    }
};

#endif