#ifndef LEVESTEIN_DFA_H
#define LEVESTEIN_DFA_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include "data_types.h"

#define STATE std::map<int, int>
#define DEFAULT_CHAR '*'

class LevesteinDFA{
public:
    LevesteinDFA();
    ~LevesteinDFA() = default;
    void write_graph_to_file(const std::string& filename);
    bool check(const std::string& word);
    void build(const std::string& word, int distance);
private:
    std::string word_;
    int max_distance_;
    Node* root_;
    std::vector<char> char_set_;
    std::map<STATE, Node*> state_to_node_;
    
    void build_recursion(STATE actual_state);
    STATE get_next_state_for_transition(STATE actual_state, char transition);
    Node* get_node(const STATE& state);
    bool is_valid_state(const STATE& state);
    void get_transitions_list_as_string(Node* base_node, const std::set<Node*>& all_nodes_list, std::vector<bool>& visited, std::string& transitions_list_str);
    int get_node_idx(Node* node, const std::set<Node*>& all_nodes_list);
    void get_nodes_tree_list_from_node(Node* base_node, std::set<Node*>& output_nodes);
};

#endif