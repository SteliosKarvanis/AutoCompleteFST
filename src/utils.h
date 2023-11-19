#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <algorithm>
#include "data_types.h"

void write_vector_to_file(const std::vector<std::string>& strings, const std::string& file, bool append = false);
bool is_sorted(const std::string& s1, const std::string& s2);
void sort_file(const std::string& input_file, const std::string& output_file);
std::vector<std::string>read_vector_from_file(const std::string& file);


//////////////////////////////////////////////////////
///////////////// Binary Search //////////////////////
//////////////////////////////////////////////////////
std::vector<std::string> binary_search(const std::string& prefix, const std::vector<std::string>& words_list);
int get_smaller_common_prefix_word_index(const std::string& word, const std::vector<std::string>& words_list, int left_idx, int right_idx);

//////////////////////////////////////////////////////
///////////////// Plot Utils /////////////////////////
//////////////////////////////////////////////////////

void write_graph_to_file(Node* base_node, const std::string& filename);
void get_transitions_list_as_string(Node* base_node, const std::set<Node*>& all_nodes_list, std::vector<bool>& visited, std::string& transitions_list_str);
int get_node_idx(Node* node, const std::set<Node*>& all_nodes_list);
void get_nodes_tree_list_from_node(Node* base_node, std::set<Node*>& output_nodes);

#endif
