#include "utils.h"

bool check_data(const std::string& filename){
    std::ifstream myfile;
    myfile.open(filename);

    std::string word;
    std::string previous_word = "";
    while(std::getline(myfile, word)){
        if(!is_sorted(previous_word, word))
            return false;
        previous_word = word;
    }
    return true;
}

std::vector<std::string>read_vector_from_file(const std::string& file){
    std::ifstream myfile(file);
    std::string word;
    std::vector<std::string> words;
    // Build
    while(std::getline(myfile, word))
        words.push_back(word);
    return words;
}

void write_vector_to_file(const std::vector<std::string>& strings, const std::string& file, bool append){
    int folder_end_idx = file.rfind("/");
    std::string folder = file.substr(0, folder_end_idx);
    std::string command = "mkdir -p " + folder;
    system(command.c_str());
    std::ofstream outfile;
    if(append){
        outfile.open(file, std::ios::out | std::ios::app);
    }
    else{
        outfile.open(file, std::ios::out);
    }
    for(const std::string& str : strings){
        outfile << str << "\n";
    }
    outfile.flush();
    outfile.close();
}


bool is_sorted(const std::string& s1, const std::string& s2){
    int index = 0;
    while(index < s1.size() && index < s2.size()){
        int value1 = s1[index];
        int value2 = s2[index];
        if(value1 > value2)
            return false;
        else if(value1 < value2){
            return true;
        }
        index++;
    }
    // Equal so far
    if(s1.size() > s2.size())
        return false;
    else
        return true;
}

void sort_file(const std::string& input_file, const std::string& output_file){
    std::ifstream myfile(input_file);
    std::string word;
    std::vector<std::string> words;
    while(std::getline(myfile, word)){
        words.push_back(word);
    }
    std::sort(words.begin(), words.end(), is_sorted);
    write_vector_to_file(words, output_file);
}

//////////////////////////////////////////////////////
///////////////// Binary Search //////////////////////
//////////////////////////////////////////////////////

/// @brief Do a binary search finding the subset of a list of words that contains words with the given prefix, 
///         returning the index range of the words [out_min_idx, out_max_idx[
/// @param prefix: the prefix to look for in the words_list 
/// @param words_list: a list of words to fetch the prefix
/// @param out_min_idx: the bottom bound of the output set
/// @param out_max_idx: the upper bound of the output set
std::vector<std::string> binary_search(const std::string& prefix, const std::vector<std::string>& words_list){
    std::vector<std::string> output_words;
    int curr_idx = get_smaller_common_prefix_word_index(prefix, words_list, 0, words_list.size());
    while(curr_idx < words_list.size() && words_list[curr_idx].substr(0, prefix.size()) == prefix){
        output_words.push_back(words_list[curr_idx]);
        curr_idx++;
    }
    return output_words;
}

int get_smaller_common_prefix_word_index(const std::string& word, const std::vector<std::string>& words_list, int left_idx, int right_idx){
    if(left_idx == right_idx)
        return left_idx;

    int middle_idx = (left_idx + right_idx) / 2;
    if(word == words_list[middle_idx])
        return middle_idx;
    else if(is_sorted(word, words_list[middle_idx]))
        return get_smaller_common_prefix_word_index(word, words_list, left_idx, middle_idx);
    else
        return get_smaller_common_prefix_word_index(word, words_list, middle_idx + 1, right_idx);
}

void write_graph_to_file(Node* base_node, const std::string& filename){
    std::set<Node*> all_nodes_list;
    get_nodes_tree_list_from_node(base_node, all_nodes_list);
    std::vector<bool> visited(all_nodes_list.size(), false);
    std::string transitions_list_str;
    get_transitions_list_as_string(base_node, all_nodes_list, visited, transitions_list_str);
    std::ofstream output_file(filename);
    output_file << transitions_list_str;
    output_file.close();
}

void get_transitions_list_as_string(Node* base_node, const std::set<Node*>& all_nodes_list, std::vector<bool>& visited, std::string& transitions_list_str){
    int base_node_idx = get_node_idx(base_node, all_nodes_list);
    visited[base_node_idx] = true;
    for(auto it = base_node->next_nodes.begin(); it != base_node->next_nodes.end(); it++){
        Node* next_node = it->second;
        int next_node_idx = get_node_idx(next_node, all_nodes_list);
        std::string transition = std::to_string(base_node_idx) + " " + std::to_string(next_node_idx) + " " + it->first + " " + std::to_string(base_node->valid) + " " + std::to_string(next_node->valid) + "\n";
        transitions_list_str = transitions_list_str + transition;
        if(!visited[next_node_idx])
            get_transitions_list_as_string(next_node, all_nodes_list, visited, transitions_list_str);
    }
}

int get_node_idx(Node* node, const std::set<Node*>& all_nodes_list){
    return std::distance(all_nodes_list.begin(), all_nodes_list.find(node));
}

void get_nodes_tree_list_from_node(Node* base_node, std::set<Node*>& output_nodes){
    bool found_node = (output_nodes.find(base_node) != output_nodes.end());
    if(!found_node)
        output_nodes.insert(base_node);
    for(auto it = base_node->next_nodes.begin(); it != base_node->next_nodes.end(); it++){
        get_nodes_tree_list_from_node(it->second, output_nodes);
    }
}