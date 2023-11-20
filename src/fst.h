#ifndef FST_H
#define FST_H

#include <string>
#include <set>
#include <sstream>
#include <fstream>
#include <iostream>
#include "data_types.h"
#include "utils.h"

class FST{
public:
    FST();
    ~FST();
    void buildFST(const std::string& filename);
    void readFST(const std::string& filename);
    std::vector<std::string> levestein(const std::string& word, int dist);
    std::vector<std::string> autocomplete(const std::string& prefix, int max_num_of_results = 20);
    void write_to_file(const std::string& filename);
    int count_nodes();
    int memory_usage();

protected:
    Node* root;
    std::vector<Node*> nodes_list;
    // Build utils
    void add_suffix(Node* base_node, const std::string& word, int common_prefix_size);
    void add_node(Node* base_node, char transition);
    void ingest_last_suffix(Node* branch_node);
    bool ingest_last_suffix_recursion(Node* actual_node);
    static Node* get_last_next_node(Node* node);
    static char get_last_next_char(Node* node);
    Node* new_word_max_existent_prefix(const std::string& new_word, int& existent_prefix_size);
    // Search utils
    void get_all_acceptable_words_from_node(Node* base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results);
    Node* node_with_prefix(const std::string& prefix);
    void levestein_dfs(std::vector<std::string>& output_words, Node* actual_node, const std::string& word, std::string curr_word, const int dist, int curr_dist, int char_idx);
};

#endif