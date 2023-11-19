#ifndef LEVESTEIN_DFA_H
#define LEVESTEIN_DFA_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include "data_types.h"
#include "utils.h"

#define STATE std::map<int, int>
#define DEFAULT_CHAR '*'

class LevesteinDFA{
public:
    LevesteinDFA();
    ~LevesteinDFA() = default;
    void write_to_file(const std::string& filename);
    bool check(const std::string& word);
    void build(const std::string& word, int distance);
private:
    std::string word_;
    int max_distance_;
    Node* root_;
    std::vector<char> char_list_;
    std::map<STATE, Node*> state_to_node_;
    
    void build_recursion(STATE actual_state);
    STATE get_next_state_for_transition(STATE actual_state, char transition);
    Node* get_node(const STATE& state);
    bool is_valid_state(const STATE& state);
};

#endif