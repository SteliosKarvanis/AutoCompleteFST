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
    void buildFST(const std::string& filename, bool should_check_data=false);
    static bool check_data(const std::string& filename);
    std::vector<std::string> retrieve_words(std::string word, int max = 20);

private:
    Node *root;
    std::vector<Node*> final_frozen_nodes;
    // Build utils
    void froze_node_tree(Node *node);
    void add_suffix(Node* base_node, std::string word);
    void add_node(Node* base_node, Transition* transition);
    void update(Node* branch_node);
    static Node *get_next_node_with_last_char(Node *node);
    static char get_next_last_char(Node* node);
    Node* get_new_word_max_common_preffix(std::string new_word);
    // Seach utils
    void DFS(Node* base_node, std::string word, std::vector<std::string>& output_words, int max);
    Node* get_max_common_prefix(std::string new_word);
    
    
};

#endif