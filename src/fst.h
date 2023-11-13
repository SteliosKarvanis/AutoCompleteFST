#ifndef FST_H
#define FST_H

#include <string>
#include <set>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "data_types.h"
#include "utils.h"

class FST{
public:
    FST();
    ~FST();
    void buildFST(const std::string& filename);
    static bool check_data(const std::string& filename);
    std::vector<std::string> build_levestein_DFA(const std::string& word, int dist);
    std::vector<std::string> autocomplete(const std::string& prefix, int max_num_of_results = 20);
    void write_graph_to_file(const std::string& filename);
    int count_nodes();
    int memory_usage();
    bool word_in_DFA(const std::string& word);

protected:
    Node* root;
    Node* final_frozen_node;
    // Build utils
    void froze_node_tree(Node* node);
    void add_suffix(Node* base_node, const std::string& word, int common_prefix_size);
    void add_node(Node* base_node, char transition);
    void ingest_last_suffix(Node* branch_node);
    static Node* get_last_next_node(Node* node);
    static char get_last_next_char(Node* node);
    Node* retrieve_new_word_max_existent_prefix(const std::string& new_word, int& existent_prefix_size);
    // Search utils
    void DFS(Node* base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results);
    Node* retrieve_node_with_prefix(const std::string& prefix);
    void levestein_dfs(std::vector<std::string>& output_words, Node* actual_node, const std::string& word, std::string curr_word, const int dist, int curr_dist, int char_idx);
    // Plot FST utils
    void get_nodes_list(Node* base_node, std::set<Node*>& output_nodes);
    int get_node_idx(Node* node, const std::set<Node*>& nodes_list);
    void get_transitions_list_as_string(Node* base_node, const std::set<Node*>& nodes_list, std::vector<bool>& visited, std::string& transitions_list_str);
    char get_previous_node_transition_by_idx(Node* actual_node, int idx);
    void delete_node_tree(Node* node);
    void reset_visited(Node* actual_node);
};

#endif