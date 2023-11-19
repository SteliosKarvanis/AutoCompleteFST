#include "levesteinDFA.h"
#include <iostream>
#include <string>
#include <vector>
#include "data_types.h"

LevesteinDFA::LevesteinDFA(std::string word, int distance){
    word_ = word;
    max_distance_ = distance;
    root_ = new Node();
    
    char_set_.push_back(DEFAULT_CHAR);
    // Initialize char_set
    for(char c: word_){
        bool found = false;
        for(char char_already_added : char_set_){
            if(c == char_already_added){
                found = true;
                break;
            }
        }
        if(!found)
            char_set_.push_back(c);
    }
    
    // Initialize state_to_node
    STATE initial_state;
    for(int i = 0; i < word_.size(); i++){
        if(i > max_distance_)
            break;
        initial_state[i] = i;
    }
    state_to_node_[initial_state] = root_;
    build();
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

void LevesteinDFA::build(){
    build_recursion(state_to_node_.begin()->first);
}

void LevesteinDFA::build_recursion(STATE actual_state){
    if(actual_state.empty())
        return;
    Node* actual_node = get_node(actual_state);
    for(char c: char_set_){
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

void LevesteinDFA::write_graph_to_file(const std::string& filename){
    std::set<Node*> all_nodes_list;
    get_nodes_tree_list_from_node(this->root_, all_nodes_list);
    std::vector<bool> visited(all_nodes_list.size(), false);
    std::string transitions_list_str;
    get_transitions_list_as_string(this->root_, all_nodes_list, visited, transitions_list_str);
    std::ofstream output_file(filename);
    output_file << transitions_list_str;
    output_file.close();
}

void LevesteinDFA::get_transitions_list_as_string(Node* base_node, const std::set<Node*>& all_nodes_list, std::vector<bool>& visited, std::string& transitions_list_str){
    int base_node_idx = get_node_idx(base_node, all_nodes_list);
    visited[base_node_idx] = true;
    for(auto it = base_node->next_nodes.begin(); it != base_node->next_nodes.end(); it++){
        Node* next_node = it->second;
        int next_node_idx = get_node_idx(next_node, all_nodes_list);
        std::string transition = std::to_string(base_node_idx) + " " + std::to_string(next_node_idx) + " " + it->first + " " + std::to_string(base_node->valid) + " " + std::to_string(next_node->valid) + "\n";
        transitions_list_str = transitions_list_str + transition;
        if(!visited[next_node_idx])
            get_transitions_list_as_string(next_node, all_nodes_list, visited, transitions_list_str);
    }
}

int LevesteinDFA::get_node_idx(Node* node, const std::set<Node*>& all_nodes_list){
    return std::distance(all_nodes_list.begin(), all_nodes_list.find(node));
}

void LevesteinDFA::get_nodes_tree_list_from_node(Node* base_node, std::set<Node*>& output_nodes){
    bool found_node = (output_nodes.find(base_node) != output_nodes.end());
    if(!found_node)
        output_nodes.insert(base_node);
    for(auto it = base_node->next_nodes.begin(); it != base_node->next_nodes.end(); it++){
        get_nodes_tree_list_from_node(it->second, output_nodes);
    }
}