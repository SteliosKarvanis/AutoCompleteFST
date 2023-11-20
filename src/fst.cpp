#include "fst.h"

FST::FST(){
    this->root = new Node();
}

FST::~FST(){
    for(auto curr_node : nodes_list)
        delete curr_node;
}

int FST::count_nodes(){
    return this->nodes_list.size();
}

int FST::memory_usage(){
    int total_memory = sizeof(nodes_list);
    for(Node* node : this->nodes_list)
        total_memory += sizeof(*node);
    return total_memory;
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// SEARCH UTILS ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> FST::autocomplete(const std::string& prefix, int max_num_of_results){
    std::vector<std::string> output_words = std::vector<std::string>();
    Node* last_preffix_node = this->node_with_prefix(prefix);
    // if the prefix is not completely found, return empty vector
    if(last_preffix_node == nullptr)
        return output_words;
    // Find words
    get_all_acceptable_words_from_node(last_preffix_node, prefix, output_words, max_num_of_results);
    return output_words;
}

void FST::get_all_acceptable_words_from_node(Node* base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results){
    if(output_words.size() == max_num_of_results)
        return;
    if(base_node->valid){
        output_words.push_back(word);
    }
    for(auto it = base_node->next_nodes.begin(); it != base_node->next_nodes.end(); it++)
        get_all_acceptable_words_from_node(it->second, word + it->first, output_words, max_num_of_results);
}

Node* FST::node_with_prefix(const std::string& prefix){
    Node* actual_node = this->root;
    for(char current_char : prefix){
        auto it = actual_node->next_nodes.find(current_char);
        if(it == actual_node->next_nodes.end())
            return nullptr;
        actual_node = it->second;
    }
    return actual_node;
}

std::vector<std::string> FST::levestein(const std::string& word, int dist){
    std::vector<std::string> output_words = std::vector<std::string>();
    this->levestein_dfs(output_words, this->root, word, "", dist, 0, 0);
    return output_words;
}

void FST::levestein_dfs(std::vector<std::string>& output_words, Node* actual_node, const std::string& word, std::string curr_word, const int dist, int curr_dist, int char_idx){
    if(curr_dist > dist)
        return;
    int dist_to_target = (int)word.size() - char_idx;
    // To may add a word, the node should be valid, and the curr_dist adding the missing suffix size should not exceed the dist
    if(actual_node->valid && curr_dist + dist_to_target <= dist){
        bool already_added = false;
        for(auto output_word : output_words){
            if(output_word == curr_word){
                already_added = true;
                break;
            }
        }
        if(!already_added)
            output_words.push_back(curr_word);
    }
    for(auto it = actual_node->next_nodes.begin(); it != actual_node->next_nodes.end(); it++){
        char transition = it->first;
        auto next_node = it->second;
        int new_dist_in_addition;
        // Each follow recursion, cover a different action when modificating the original word
        if(char_idx < word.size() && word[char_idx] == transition){
            // Match (Follow a transition, making a char match)
            this->levestein_dfs(output_words, next_node, word, curr_word + transition, dist, curr_dist, char_idx + 1);
        }
        else{
            // Replace (Follow a transition, going to the next char to be analised)
            this->levestein_dfs(output_words, next_node, word, curr_word + transition, dist, curr_dist + 1, char_idx + 1);
            // Addition (Follow a transition, remaining the char to be analised)
            this->levestein_dfs(output_words, next_node, word, curr_word + transition, dist, curr_dist + 1, char_idx);
            // Deleting (Stay in the state, skipping the current char to be analysed)
            this->levestein_dfs(output_words, actual_node, word, curr_word, dist, curr_dist + 1, char_idx + 1);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// BUILD UTILS ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////


void FST::readFST(const std::string& filename){
    std::string line;
    std::ifstream myfile(filename);

    nodes_list.push_back(new Node());
    this->root = nodes_list[0];
    while(std::getline(myfile, line)){
        std::stringstream ss(line);
        int base_node_idx, next_node_idx;
        char transition;
        bool base_node_valid, next_node_valid;
        ss >> base_node_idx >> next_node_idx >> transition >> base_node_valid >> next_node_valid;
        while(base_node_idx >= (int)nodes_list.size())
            nodes_list.push_back(new Node());
        while(next_node_idx >= (int)nodes_list.size())
            nodes_list.push_back(new Node());
        nodes_list[base_node_idx]->valid = base_node_valid;
        nodes_list[next_node_idx]->valid = next_node_valid;
        nodes_list[base_node_idx]->next_nodes.insert(std::pair<char, Node*>(transition, nodes_list[next_node_idx]));
    }
}

void FST::buildFST(const std::string& filename){
    std::string word;
    int existent_prefix_size;
    Node* last_common_node;
    std::ifstream myfile(filename);
    while(std::getline(myfile, word)){
        last_common_node = new_word_max_existent_prefix(word, existent_prefix_size);
        this->ingest_last_suffix(last_common_node);
        this->add_suffix(last_common_node, word, existent_prefix_size);
    }
    this->ingest_last_suffix(this->root);
    this->nodes_list.push_back(this->root);
}

Node* FST::get_last_next_node(Node* node){
    if(node->next_nodes.empty())
        return nullptr;
    return node->next_nodes.rbegin()->second;
}

char FST::get_last_next_char(Node* node){
    return node->next_nodes.rbegin()->first;
}

void FST::add_node(Node* base_node, char transition){
    Node* new_node = new Node();
    base_node->next_nodes.insert(std::pair<char, Node*>(transition, new_node));
}

Node* FST::new_word_max_existent_prefix(const std::string& new_word, int& existent_prefix_size){
    Node* actual_node = this->root;
    existent_prefix_size = 0;
    for(char current_char : new_word){
        if(actual_node->next_nodes.empty() || get_last_next_char(actual_node) != current_char)
            break;
        existent_prefix_size++;
        actual_node = get_last_next_node(actual_node);
    }
    return actual_node;
}

void FST::add_suffix(Node* base_node, const std::string& word, int common_prefix_size){
    Node* actual_node = base_node;
    for(int curr_index = common_prefix_size; curr_index < word.size(); curr_index++){
        this->add_node(actual_node, word[curr_index]);
        actual_node = get_last_next_node(actual_node);
    }
    actual_node->valid = true;
}

void FST::ingest_last_suffix(Node* branch_node){
    // Do nothing if the branch node have no next nodes 
    // Cases where the word been added is a continuation of the last word added
    if(branch_node->next_nodes.empty())
        return;

    ingest_last_suffix_recursion(branch_node);
}

bool FST::ingest_last_suffix_recursion(Node* actual_node){
    auto next_node = get_last_next_node(actual_node);
    if(next_node == nullptr)
        return true;
    bool equal = ingest_last_suffix_recursion(next_node);
    if(equal){
        for(Node* node : this->nodes_list){
            if(node->equals(next_node)){
                actual_node->next_nodes[get_last_next_char(actual_node)] = node;
                delete next_node;
                return true;
            }
        }
    }
    this->nodes_list.push_back(next_node);
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// PLOT UTILS /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void FST::write_to_file(const std::string& filename){
    write_graph_to_file(this->root, filename);
}
