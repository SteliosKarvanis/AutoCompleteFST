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
    static Node *get_next_node_with_last_char(Node *node);
    static char get_next_last_char(Node* node);
    static void check_data(const std::string& filename);
    static bool is_sorted(std::string s1, std::string s2);
    void print_fst();

private:
    Node *root;
    std::vector<Node*> final_frozen_nodes;
    void froze_node_tree(Node *node);
    void dummy_add_word(std::string word);
};

#endif