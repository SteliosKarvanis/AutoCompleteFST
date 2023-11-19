#include "levesteinDFA.h"
#include <iostream>
#include <string>
#include <vector>
#include "data_types.h"

LevesteinDFA::LevesteinDFA(){}

void LevesteinDFA::build(const std::string& word, int distance){
    word_ = word;
    max_distance_ = distance;
    root_ = new Node();
    char_list_.clear();
    char_list_.shrink_to_fit();
    char_list_.push_back(DEFAULT_CHAR);
    // Initialize char_set
    for(char c: word_){
        bool found = false;
        for(char char_already_added : char_list_){
            if(c == char_already_added){
                found = true;
                break;
            }
        }
        if(!found)
            char_list_.push_back(c);
    }
    
    // Initialize state_to_node
    STATE initial_state;
    for(int i = 0; i < word_.size(); i++){
        if(i > max_distance_)
            break;
        initial_state[i] = i;
    }
    state_to_node_.clear();
    state_to_node_[initial_state] = root_;
    build_recursion(state_to_node_.begin()->first);
}

bool LevesteinDFA::check(const std::string& word){
    Node* actual_node = root_;
    for(char c : word){
        bool found = false;
        for(auto it = actual_node->next_nodes.begin(); it != actual_node->next_nodes.end(); it++){
            if(it->first == c){
                found = true;
                actual_node = it->second;
                break;
            }
        }
        if(actual_node->next_nodes.empty())
            break;
        if(!found)
            actual_node = actual_node->next_nodes.find(DEFAULT_CHAR)->second;
    }
    return actual_node->valid;
}
void LevesteinDFA::build_recursion(STATE actual_state){
    if(actual_state.empty())
        return;
    Node* actual_node = get_node(actual_state);
    for(char c: char_list_){
        STATE next_state = get_next_state_for_transition(actual_state, c);
        Node* next_node = get_node(next_state);
        if(next_node == nullptr){
            next_node = new Node();
            next_node->valid = is_valid_state(next_state);
            state_to_node_[next_state] = next_node;
            build_recursion(next_state);        
        }
        // If is adding a transition to a node that already have a * transition
        if(actual_node->next_nodes.find(DEFAULT_CHAR) != actual_node->next_nodes.end()){
            if(actual_node->next_nodes[DEFAULT_CHAR] == next_node)
                continue;
        }
        // If is adding a * transition to a node
        if(c == DEFAULT_CHAR){
            for(auto it = actual_node->next_nodes.rbegin(); it != actual_node->next_nodes.rend(); it++){
                if(it->second == next_node){
                    actual_node->next_nodes.erase(it->first);
                    it--;
                }
            }
            if(actual_node->next_nodes.begin()->second == next_node)
                actual_node->next_nodes.erase(actual_node->next_nodes.begin());
        }
        actual_node->next_nodes[c] = next_node;
    }
}

STATE LevesteinDFA::get_next_state_for_transition(STATE actual_state, char transition){
    STATE new_state;
    if(actual_state.begin()->first == 0){
        if(actual_state[0] < max_distance_)
            new_state[0] = actual_state[0] + 1;
    }
    for(int i = 1; i < word_.size() + 1; i++){
        // Deletetion
        int deletion_dist = max_distance_ + 1;
        if(new_state.find(i-1) != new_state.end())
            deletion_dist = std::min(new_state[i-1] + 1, max_distance_ + 1);
        
        // Addition
        int addition_dist = max_distance_ + 1;
        if(actual_state.find(i) != actual_state.end())
            addition_dist = std::min(actual_state[i] + 1, max_distance_ + 1);
        
        // Replace or match
        int replace_dist = max_distance_ + 1;
        if(actual_state.find(i-1) != actual_state.end()){
            char correct_char = word_[i-1];
            int cost = (transition == correct_char) ? 0 : 1;
            replace_dist = std::min(actual_state[i-1] + cost, max_distance_ + 1);
        }
        // Global dist
        int dist = std::min(deletion_dist, std::min(addition_dist, replace_dist));
        if(dist < max_distance_ + 1)
            new_state[i] = dist;
    }
    return new_state;
}

Node* LevesteinDFA::get_node(const STATE& state){
    if(state_to_node_.find(state) == state_to_node_.end()){
        return nullptr;
    }
    return state_to_node_[state];
}

bool LevesteinDFA::is_valid_state(const STATE& state){
    return (state.find((int)word_.size()) != state.end());
}

void LevesteinDFA::write_to_file(const std::string& filename){
    write_graph_to_file(root_, filename);
}