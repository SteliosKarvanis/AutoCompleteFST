#ifndef FST_H
#define FST_H

#include <string>
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

private:
    shp<Node> root;
    shp<Node> final_frozen_node;
    // Build utils
    void froze_node_tree(const shp<Node>& node);
    void add_suffix(const shp<Node>& base_node, const std::string& word, int common_prefix_size);
    static void add_node(shp<Node>& base_node, const shp<Transition>& transition);
    void ingest_last_suffix(const shp<Node>& branch_node);
    static shp<Node> get_last_next_node(const shp<Node>& node);
    static char get_last_next_char(const shp<Node>& node);
    shp<Node> retrieve_new_word_max_existent_prefix(const std::string& new_word, int& existent_prefix_size);
    // Search utils
    void DFS(const shp<Node>& base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results);
    shp<Node> retrieve_node_with_prefix(const std::string& prefix);
    void levestein_dfs(std::vector<std::string>& output_words, const shp<Node>& actual_node, const std::string& word, const std::string& curr_word, int dist, int curr_dist, int char_idx);
    // Plot FST utils
    void get_nodes_list(const shp<Node>& base_node, std::vector<shp<Node>>& output_nodes);
    static int get_node_idx(const shp<Node>& node, const std::vector<shp<Node>>& nodes_list);
    void get_transitions_list_as_string(const shp<Node>& base_node, const std::vector<shp<Node>>& nodes_list, std::vector<bool>& visited, std::string& transitions_list_str);
};

#endif