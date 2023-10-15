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
    void add_suffix(Node* base_node, const std::string& word, int common_prefix_size);
    void add_node(Node* base_node, Transition* transition);
    void update(Node* branch_node);
    static Node* get_last_next_node(Node* node);
    static char get_last_next_char(Node* node);
    Node* get_new_word_max_existent_prefix(const std::string& new_word, int& existent_prefix_size);
    // Search utils
    void DFS(Node* base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results);
    Node* retrieve_node_with_prefix(const std::string& prefix);

};

#endif