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
    std::vector<std::string> levestein(const std::string& word, int dist);
    std::vector<std::string> autocomplete(const std::string& prefix, int max_num_of_results = 20);
    void write_graph_to_file(const std::string& filename);
    int count_nodes();
    int memory_usage();

protected:
    Node* root;
    Node* final_frozen_node;
    std::set<Node*> nodes_list;
    // Build utils
    void add_suffix(Node* base_node, const std::string& word, int common_prefix_size);
    void add_node(Node* base_node, char transition);
    void ingest_last_suffix(Node* branch_node);
    bool ingest_last_suffix_recursion(Node* actual_node);
    bool compare_nodes(Node* actual_node, Node* new_node);
    static Node* get_last_next_node(Node* node);
    static char get_last_next_char(Node* node);
    Node* new_word_max_existent_prefix(const std::string& new_word, int& existent_prefix_size);
    void delete_node_tree(Node* node);
    // Search utils
    void get_all_acceptable_words_from_node(Node* base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results);
    Node* node_with_prefix(const std::string& prefix);
    void levestein_dfs(std::vector<std::string>& output_words, Node* actual_node, const std::string& word, std::string curr_word, const int dist, int curr_dist, int char_idx);
    // Plot FST utils
    void get_nodes_tree_list_from_node(Node* base_node, std::set<Node*>& output_nodes);
    int get_node_idx(Node* node, const std::set<Node*>& all_nodes_list);
    void get_transitions_list_as_string(Node* base_node, const std::set<Node*>& all_nodes_list, std::vector<bool>& visited, std::string& transitions_list_str);
};

#endif