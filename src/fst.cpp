#include "fst.h"

FST::FST(){
    this->root = new Node();
    this->final_frozen_node = nullptr;
}

FST::~FST(){
    delete_node_tree(this->root);
}

int FST::count_nodes(){
    std::set<Node*> nodes_list;
    get_nodes_list(this->root, nodes_list);
    return nodes_list.size();
}

int FST::memory_usage(){
    std::set<Node*> nodes_list;
    get_nodes_list(this->root, nodes_list);
    int total_memory = 0;
    for(Node* node : nodes_list)
        total_memory += sizeof(*node);
    return total_memory;
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// SEARCH UTILS ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/// @brief Get the words that have the given word as a prefix
/// @param prefix: The prefix to search for
/// @param max_num_of_results: The maximum number of results to return (To avoid returning too many results for small prefixes)
/// @return a vector of strings that are the words that have the given word as a prefix
std::vector<std::string> FST::autocomplete(const std::string& prefix, int max_num_of_results){
    std::vector<std::string> output_words = std::vector<std::string>();
    Node* last_preffix_node = this->retrieve_node_with_prefix(prefix);
    // if the prefix is not completely found, return empty vector
    if(last_preffix_node == nullptr)
        return output_words;
    // Find words
    get_all_acceptable_words_from_node(last_preffix_node, prefix, output_words, max_num_of_results);
    return output_words;
}

/// @brief Run a DFS on the FST to find all the words that have the given word as a prefix
/// @param base_node: the node to start from, should be the node that has the given word as a prefix
/// @param word: the current word build so far, from the root
/// @param output_words: the output, the words that have the given word as a prefix 
/// @param max_num_of_results: the maximum number of results to return (To avoid returning too many results for small prefixes)
void FST::get_all_acceptable_words_from_node(Node* base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results){
    if(output_words.size() == max_num_of_results)
        return;
    if(base_node->valid){
        output_words.push_back(word);
    }
    for(int i = 0; i < base_node->next_nodes.size(); i++){
        Node* next_node = base_node->next_nodes[i];
        get_all_acceptable_words_from_node(next_node, word + base_node->forward_transitions[i], output_words, max_num_of_results);
    }
}

/// @brief Retrieve the node that gives the requested prefix
/// @param prefix: the prefix to search for
/// @return the node that gives the requested prefix or nullptr if no node found
Node* FST::retrieve_node_with_prefix(const std::string& prefix){
    Node* actual_node = this->root;
    for(char current_char : prefix){
        bool found = false;
        for(int i = 0; i < actual_node->forward_transitions.size(); i++){
            if(actual_node->forward_transitions[i] == current_char){
                actual_node = actual_node->next_nodes[i];
                found = true;
                break;
            }
        }
        if(!found)
            return nullptr;
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
    for(int i = 0; i < actual_node->next_nodes.size(); i++){
        char transition = actual_node->forward_transitions[i];
        auto next_node = actual_node->next_nodes[i];
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

/// @brief Build the FST from the words in the given file
/// @param filename: the file with the words (must be ordered, to check it, use `check_data()`)
void FST::buildFST(const std::string& filename){
    std::string word;
    int existent_prefix_size;
    Node* last_common_node;
    std::ifstream myfile(filename);
    int i = 0;
    while(std::getline(myfile, word)){
        last_common_node = retrieve_new_word_max_existent_prefix(word, existent_prefix_size);
        this->ingest_last_suffix(last_common_node);
        this->froze_node_tree(get_last_next_node(last_common_node));
        this->add_suffix(last_common_node, word, existent_prefix_size);
        //this->write_graph_to_file("../graph_" + std::to_string(i) + ".txt");
        i++;
        std::cout << i << std::endl;
    }
    this->ingest_last_suffix(this->root);
    //this->write_graph_to_file("../graph_" + std::to_string(i) + ".txt");
}

/// @brief Froze all the node tree from the given node (including the base node)
/// @param node: the base node to froze the tree from
void FST::froze_node_tree(Node* node){
    // if node is already frozen, all her tree should be frozen
    if(node == nullptr || node->frozen)
        return;

    node->frozen = true;
    // Update the final frozen node
    if(node->valid && node->next_nodes.empty() && this->final_frozen_node == nullptr)
        this->final_frozen_node = node;

    // Recursively froze the tree
    for(auto next_node : node->next_nodes){
        if(next_node->frozen)
            continue;
        froze_node_tree(next_node);
    }
}

/// @brief Get the next node added to the given node (usefull in FST build)
Node* FST::get_last_next_node(Node* node){
    if(node->next_nodes.empty())
        return nullptr;
    return node->next_nodes[node->next_nodes.size() - 1];
}

/// @brief Get the char related to the last transition from the given node (usefull in FST build)
char FST::get_last_next_char(Node* node){
    return node->forward_transitions[node->forward_transitions.size() - 1];
}

/// @brief Add a new node to the fst, building all the transaction related
/// @param base_node: the parent node to the new node
/// @param transition: the transition to the new node
void FST::add_node(Node* base_node, char transition){
    Node* new_node = new Node();

    base_node->next_nodes.push_back(new_node);
    base_node->forward_transitions.push_back(transition);
}

/// @brief Get the node with the max common prefix with the new word
/// @param new_word: the word that is been added
/// @param existent_prefix_size: output, the size of the common prefix
/// @return the node with the max common prefix with the new word
Node* FST::retrieve_new_word_max_existent_prefix(const std::string& new_word, int& existent_prefix_size){
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

/// @brief Add the suffix of the new word to the fst
/// @param base_node: should be the node with the biggest common prefix with the new word
/// @param word: the word that is been added
/// @param common_prefix_size: the size of the biggest common prefix
void FST::add_suffix(Node* base_node, const std::string& word, int common_prefix_size){
    Node* actual_node = base_node;
    for(int curr_index = common_prefix_size; curr_index < word.size(); curr_index++){
        this->add_node(actual_node, word[curr_index]);
        actual_node = get_last_next_node(actual_node);
    }
    actual_node->valid = true;
}

/// @brief Before add a new word, updates the fst, by joining the new word suffix with a previous frozen word suffix
/// @param branch_node: the node with the biggest common prefix with the new word that will been added 
void FST::ingest_last_suffix(Node* branch_node){
    // Do nothing if there is no frozen node yet 
    // Cases where the word been added is the first word, or the first word with a new suffix
    if(final_frozen_node == nullptr)
        return;
    // Do nothing if the branch node have no next nodes 
    // Cases where the word been added is a continuation of the last word added
    if(branch_node->next_nodes.empty())
        return;

    // Get last node added previously
    std::vector<Node*> new_nodes_list;
    new_nodes_list.push_back(branch_node);
    Node* last_new_node = branch_node;
    while(!last_new_node->next_nodes.empty()){
        last_new_node = get_last_next_node(last_new_node);
        new_nodes_list.push_back(last_new_node);
    }
    int curr_idx = (int)new_nodes_list.size();
    int max_common_suffix_size = -1;
    Node* max_suffix_node = nullptr;
    Node* new_node_with_suffix = nullptr;
    ingest_last_suffix_dfs(root, new_nodes_list, curr_idx, max_suffix_node, new_node_with_suffix, max_common_suffix_size);
    delete_node_tree(new_node_with_suffix->next_nodes[new_node_with_suffix->next_nodes.size() - 1]);
    new_node_with_suffix->next_nodes[new_node_with_suffix->next_nodes.size() - 1] = max_suffix_node;
}

bool FST::ingest_last_suffix_dfs(Node* actual_node, const std::vector<Node*>& new_nodes_list, int& curr_new_node_idx, Node*& max_suffix_node, Node*& max_suffix_new_node, int& max_suffix_size){
    // If is the final node
    if(actual_node == final_frozen_node){
        curr_new_node_idx = (int)new_nodes_list.size() - 2;
        return true;
    }
    for(int i = 0; i < actual_node->next_nodes.size(); i++){
        bool valid = ingest_last_suffix_dfs(actual_node->next_nodes[i], new_nodes_list, curr_new_node_idx, max_suffix_node, max_suffix_new_node, max_suffix_size);
        if(!valid)
            continue;
        Node* new_node = new_nodes_list[curr_new_node_idx];
        char transition = actual_node->forward_transitions[i];
        bool nodes_equal = compare_nodes(actual_node, new_node, transition);
        if(nodes_equal && curr_new_node_idx > 0){
            curr_new_node_idx--;
            return true;
        }
        // Update transitions
        int common_suffix_nodes_size = (int)new_nodes_list.size() - curr_new_node_idx;
        if(max_suffix_size == -1 || common_suffix_nodes_size > max_suffix_size){
            max_suffix_size = common_suffix_nodes_size;
            max_suffix_new_node = new_node;
            max_suffix_node = actual_node->next_nodes[i];
        }
        return false;
    }
    return false;
}

bool FST::compare_nodes(Node* actual_node, Node* new_node, char transition){
    if((int)actual_node->next_nodes.size() > 1 || (int)new_node->next_nodes.size() > 1)
        return false;
    if(!actual_node->frozen)
        return false;
    if((actual_node->valid != new_node->valid))
        return false;
    if(new_node->forward_transitions[new_node->forward_transitions.size() - 1] != transition)
        return false;
    return true;
}

void FST::delete_node_tree(Node* node){
    std::set<Node*> nodes_list;
    get_nodes_list(node, nodes_list);
    for(auto curr_node : nodes_list)
        delete curr_node;
}

/// @brief Check if the file contains sorted words
/// @return if the file is sorted
bool FST::check_data(const std::string& filename){
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

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// PLOT UTILS /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/// @brief write graph to a txt file, write all transitions in the format: "base_node_idx next_node_idx transition_char"
/// @param filename: the file to write the graph
void FST::write_graph_to_file(const std::string& filename){
    std::set<Node*> nodes_list;
    get_nodes_list(this->root, nodes_list);
    std::vector<bool> visited(nodes_list.size(), false);
    std::string transitions_list_str;
    get_transitions_list_as_string(this->root, nodes_list, visited, transitions_list_str);
    std::ofstream output_file(filename);
    output_file << transitions_list_str;
    output_file.close();
}

void FST::get_transitions_list_as_string(Node* base_node, const std::set<Node*>& nodes_list, std::vector<bool>& visited, std::string& transitions_list_str){
    int base_node_idx = get_node_idx(base_node, nodes_list);
    visited[base_node_idx] = true;
    for(int i = 0; i < base_node->next_nodes.size(); i++){
        Node* next_node = base_node->next_nodes[i];
        int next_node_idx = get_node_idx(next_node, nodes_list);
        std::string transition = std::to_string(base_node_idx) + " " + std::to_string(next_node_idx) + " " + base_node->forward_transitions[i] + " " + std::to_string(base_node->valid) + " " + std::to_string(next_node->valid) + "\n";
        transitions_list_str = transitions_list_str + transition;
        if(!visited[next_node_idx])
            get_transitions_list_as_string(next_node, nodes_list, visited, transitions_list_str);
    }
}

int FST::get_node_idx(Node* node, const std::set<Node*>& nodes_list){
    return std::distance(nodes_list.begin(), nodes_list.find(node));
}


void FST::get_nodes_list(Node* base_node, std::set<Node*>& output_nodes){
    bool found_node = (output_nodes.find(base_node) != output_nodes.end());
    if(!found_node)
        output_nodes.insert(base_node);

    for(int i = 0; i < base_node->next_nodes.size(); i++){
        Node* next_node = base_node->next_nodes[i];
        get_nodes_list(next_node, output_nodes);
    }
}