#ifndef FST_H
#define FST_H

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "data_types.h"

class FST{
public:
    FST();
    ~FST() = default;
    void buildFST(const std::string& filename);
    static bool check_data(const std::string& filename);
    std::vector<std::string> retrieve_words(const std::string& prefix, int max_num_of_results = 20);

private:
    Node* root;
    Node* final_frozen_node;
    // Build utils
    void froze_node_tree(Node* node);
    void add_suffix(Node* base_node, const std::string& word, int initial_depth);
    void add_node(Node* base_node, Transition* transition);
    void update(Node* branch_node);
    static Node* get_next_node_with_last_char(Node* node);
    static char get_next_last_char(Node* node);
    Node* get_new_word_max_common_preffix(const std::string& new_word, int& common_prefix_size);
    // Search utils
    void DFS(Node* base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results);
    Node* retrieve_node_with_prefix(const std::string& prefix);
    std::string get_last_word_suffix(Node* node);

};

#endif