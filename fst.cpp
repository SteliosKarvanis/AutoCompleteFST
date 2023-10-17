#include "fst.h"
#include "sort.h"


FST::FST(){
    this->root = new Node();
    this->final_frozen_node = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// SEARCH UTILS ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/// @brief Get the words that have the given word as a prefix
/// @param prefix: The prefix to search for
/// @param max_num_of_results: The maximum number of results to return (To avoid returning too many results for small prefixes)
/// @return a vector of strings that are the words that have the given word as a prefix
std::vector<std::string> FST::retrieve_words(const std::string& prefix, int max_num_of_results){
    std::vector<std::string> output_words = std::vector<std::string>();
    Node* last_preffix_node = this->retrieve_node_with_prefix(prefix);
    // if the prefix is not completely found, return empty vector
    if(last_preffix_node == nullptr)
        return output_words;
    // Find words
    DFS(last_preffix_node, prefix, output_words, max_num_of_results);
    return output_words;
}

/// @brief Run a DFS on the FST to find all the words that have the given word as a prefix
/// @param base_node: the node to start from, should be the node that has the given word as a prefix
/// @param word: the current word build so far, from the root
/// @param output_words: the output, the words that have the given word as a prefix 
/// @param max_num_of_results: the maximum number of results to return (To avoid returning too many results for small prefixes)
void FST::DFS(Node* base_node, const std::string& word, std::vector<std::string>& output_words, int max_num_of_results){
    if(output_words.size() == max_num_of_results)
        return;
    if(base_node->valid){
        output_words.push_back(word);
    }
    for(int i = 0; i < base_node->next_nodes.size(); i++){
        Node* next_node = base_node->next_nodes[i];
        Transition* next_transition = base_node->forward_transitions[i];
        DFS(next_node, word + next_transition->character, output_words, max_num_of_results);
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
            if(actual_node->forward_transitions[i]->character == current_char){
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
        auto transition = actual_node->forward_transitions[i];
        auto next_node = actual_node->next_nodes[i];
        int new_dist_in_addition;
        // Each follow recursion, cover a different action when modificating the original word
        if(char_idx < word.size() && word[char_idx] == transition->character){
            // Match (Follow a transition, making a char match)
            this->levestein_dfs(output_words, next_node, word, curr_word + transition->character, dist, curr_dist, char_idx + 1);
        }
        else{
            // Replace (Follow a transition, going to the next char to be analised)
            this->levestein_dfs(output_words, next_node, word, curr_word + transition->character, dist, curr_dist + 1, char_idx + 1);
            // Addition (Follow a transition, remaining the char to be analised)
            this->levestein_dfs(output_words, next_node, word, curr_word + transition->character, dist, curr_dist + 1, char_idx);
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
    while(std::getline(myfile, word)){
        last_common_node = retrieve_new_word_max_existent_prefix(word, existent_prefix_size);
        this->update(last_common_node);
        this->froze_node_tree(get_last_next_node(last_common_node));
        this->add_suffix(last_common_node, word, existent_prefix_size);
    }
    this->update(this->root);
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
    return node->forward_transitions[node->forward_transitions.size() - 1]->character;
}

/// @brief Add a new node to the fst, building all the transaction related
/// @param base_node: the parent node to the new node
/// @param transition: the transition to the new node
void FST::add_node(Node* base_node, Transition* transition){
    Node* new_node = new Node();

    base_node->next_nodes.push_back(new_node);
    base_node->forward_transitions.push_back(transition);

    new_node->previous_nodes.push_back(base_node);
    new_node->backward_transitions.push_back(transition);
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
        char current_char = word[curr_index];
        Transition* transition = new Transition(current_char);
        this->add_node(actual_node, transition);
        actual_node = get_last_next_node(actual_node);
    }
    actual_node->valid = true;
}

/// @brief Before add a new word, updates the fst, by joining the new word suffix with a previous frozen word suffix
/// @param branch_node: the node with the biggest common prefix with the new word that will been added 
void FST::update(Node* branch_node){
    // Do nothing if there is no frozen node yet 
    // Cases where the word been added is the first word, or the first word with a new suffix
    if(final_frozen_node == nullptr)
        return;
    // Do nothing if the branch node have no next nodes 
    // Cases where the word been added is a continuation of the last word added
    if(branch_node->next_nodes.empty())
        return;

    // Get last node added previously
    Node* last_new_node = branch_node;
    while(!last_new_node->next_nodes.empty())
        last_new_node = get_last_next_node(last_new_node);

    // Get an frozen node equals to a related no frozen node that can be replaced by the first one
    Node* actual_new_node = last_new_node;
    Node* actual_frozen_node = final_frozen_node;
    Node* next_node;
    // In each iteration, compares if actual_new_node and actual_frozen_node are equal and if both are valid for a new transition
    while(actual_new_node != branch_node){
        next_node = nullptr;
        char curr_desired_char = actual_new_node->backward_transitions[actual_new_node->backward_transitions.size() - 1]->character;
        Node* next_new_node = actual_new_node->previous_nodes[actual_new_node->previous_nodes.size() - 1];
        // The branch node is not part of the suffix that was added and the next node must not have branches
        if(next_new_node == branch_node || next_new_node->next_nodes.size() > 1)
            break;
        for(int i = 0; i < actual_frozen_node->previous_nodes.size(); i++){
            Node* previous_node = actual_frozen_node->previous_nodes[i];
            // The previous node must be frozen
            if(!previous_node->frozen)
                continue;
            // The previous node must have the same valid state as the next_new_node
            if(previous_node->valid != next_new_node->valid)
                continue;
            // The previous node must not have branches
            if(previous_node->next_nodes.size() > 1)
                continue;

            if(actual_frozen_node->backward_transitions[i]->character == curr_desired_char){
                next_node = previous_node;
                break;
            }
        }
        // No equals node found
        if(next_node == nullptr)
            break;
        actual_frozen_node = next_node;
        actual_new_node = next_new_node;
    }

    // Updates the transitions
    actual_new_node = actual_new_node->previous_nodes[actual_new_node->previous_nodes.size() - 1];
    actual_new_node->next_nodes[actual_new_node->next_nodes.size() - 1] = actual_frozen_node;
    actual_frozen_node->previous_nodes.push_back(actual_new_node);
    actual_frozen_node->backward_transitions.push_back(actual_new_node->forward_transitions[actual_new_node->forward_transitions.size() - 1]);
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