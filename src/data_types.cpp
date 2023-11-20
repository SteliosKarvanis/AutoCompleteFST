#include "data_types.h"

bool compare_nodes(Node* actual_node, Node* new_node){
    if((actual_node->valid != new_node->valid))
        return false;
    if(new_node->next_nodes != actual_node->next_nodes)
        return false;
    return true;
}