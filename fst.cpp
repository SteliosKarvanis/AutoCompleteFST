#include "fst.h"
#include "sort.h"


FST::FST(){
    this->root = new Node();
    this->final_frozen_node = nullptr;
}

void FST::buildFST(const std::string& filename, bool should_check_data){
    if(should_check_data){
        if(!check_data(filename)){
            std::cout << "Invalid Data" << std::endl;
            return;
        }
    }

    std::ifstream myfile;
    std::string word;
    int common_prefix_size;
    Node* last_common_node;
    myfile.open(filename);
    while(std::getline(myfile, word)){
        last_common_node = get_new_word_max_common_preffix(word, common_prefix_size);
        this->update(last_common_node);
        this->froze_node_tree(get_next_node_with_last_char(last_common_node));
        this->add_suffix(last_common_node, word, common_prefix_size);
    }
    this->update(last_common_node);
}

std::vector<std::string> FST::retrieve_words(std::string word, int max){
    std::vector<std::string> output_words = std::vector<std::string>();
    int depth;
    Node* last_preffix_node = this->get_max_common_prefix(word, depth);
    if(depth != word.size())
        return output_words;
    // Find words
    DFS(last_preffix_node, word, output_words, max);
    return output_words;
}

void FST::DFS(Node* base_node, std::string word, std::vector<std::string>& output_words, int max){
    if(output_words.size() == max)
        return;
    if(base_node->valid){
        output_words.push_back(word);
    }
    for(int i = 0; i < base_node->next_nodes.size(); i++){
        Node* next_node = base_node->next_nodes[i];
        Transition* next_transition = base_node->forward_transitions[i];
        DFS(next_node, word + next_transition->character, output_words, max);
    }
}

void FST::froze_node_tree(Node* node){
    if(node == nullptr || node->frozen)
        return;

    node->frozen = true;
    if(node->valid && node->next_nodes.empty() && this->final_frozen_node == nullptr)
        this->final_frozen_node = node;
    for(auto next_node : node->next_nodes){
        if(next_node->frozen)
            continue;
        froze_node_tree(next_node);
    }
}

Node* FST::get_next_node_with_last_char(Node* node){
    if(node->next_nodes.empty())
        return nullptr;
    return node->next_nodes[node->next_nodes.size() - 1];
}

char FST::get_next_last_char(Node* node){
    return node->forward_transitions[node->forward_transitions.size() - 1]->character;
}

void FST::add_node(Node* base_node, Transition* transition){
    Node* new_node = new Node();

    base_node->next_nodes.push_back(new_node);
    base_node->forward_transitions.push_back(transition);

    new_node->previous_nodes.push_back(base_node);
    new_node->backward_transitions.push_back(transition);
}

Node* FST::get_new_word_max_common_preffix(std::string new_word, int& common_prefix_size){
    Node* actual_node = this->root;
    common_prefix_size = 0;
    for(char current_char : new_word){
        if(actual_node->next_nodes.empty() || get_next_last_char(actual_node) != current_char)
            break;
        common_prefix_size++;
        actual_node = get_next_node_with_last_char(actual_node);
    }
    return actual_node;
}

Node* FST::get_max_common_prefix(const std::string& word, int& depth){
    Node* actual_node = this->root;
    depth = 0;
    for(char current_char : word){
        bool found = false;
        for(int i = 0; i < actual_node->forward_transitions.size(); i++){
            if(actual_node->forward_transitions[i]->character == current_char){
                actual_node = actual_node->next_nodes[i];
                found = true;
                depth++;
                break;
            }
        }
        if(!found)
            break;
    }
    return actual_node;
}

void FST::add_suffix(Node* base_node, std::string word, int initial_depth){
    Node* actual_node = base_node;
    for(int curr_index = initial_depth; curr_index < word.size(); curr_index++){
        // if no next edge found, add the node
        char current_char = word[curr_index];
        Transition* transition = new Transition(current_char);
        this->add_node(actual_node, transition);
        actual_node = get_next_node_with_last_char(actual_node);
    }
    actual_node->valid = true;
}

std::string FST::get_last_word_suffix(Node* node){
    std::string suffix = "";
    Node* actual_node = node;
    while(!actual_node->next_nodes.empty()){
        int forward_transitions_size = actual_node->forward_transitions.size();
        suffix.push_back(actual_node->forward_transitions[forward_transitions_size - 1]->character);
        actual_node = actual_node->next_nodes[forward_transitions_size - 1];
    }
    return suffix;
}

void FST::update(Node* branch_node){
    if(final_frozen_node == nullptr)
        return;
    if(branch_node->next_nodes.empty())
        return;
    // Get last node
    Node* last_new_node = branch_node;
    while(!last_new_node->next_nodes.empty())
        last_new_node = get_next_node_with_last_char(last_new_node);

    Node* actual_new_node = last_new_node;
    Node* actual_frozen_node = final_frozen_node;
    Node* next_node;
    while(actual_new_node != branch_node){
        next_node = nullptr;
        char curr_desired_char = actual_new_node->backward_transitions[actual_new_node->backward_transitions.size() - 1]->character;
        Node* next_new_node = actual_new_node->previous_nodes[actual_new_node->previous_nodes.size() - 1];
        if(next_new_node == branch_node)
            break;
        for(int i = 0; i < actual_frozen_node->previous_nodes.size(); i++){
            if(!actual_frozen_node->previous_nodes[i]->frozen)
                continue;
            if(actual_frozen_node->previous_nodes[i]->valid != next_new_node->valid)
                continue;
            if(actual_frozen_node->backward_transitions[i]->character == curr_desired_char){
                next_node = actual_frozen_node->previous_nodes[i];
                break;
            }
        }
        if(next_node == nullptr)
            break;
        if(next_node->next_nodes.size() > 1 || next_new_node->next_nodes.size() > 1)
            break;
        actual_frozen_node = next_node;
        actual_new_node = next_new_node;
    }
    actual_new_node = actual_new_node->previous_nodes[actual_new_node->previous_nodes.size() - 1];
    actual_new_node->next_nodes[actual_new_node->next_nodes.size() - 1] = actual_frozen_node;
    actual_frozen_node->previous_nodes.push_back(actual_new_node);
    actual_frozen_node->backward_transitions.push_back(actual_new_node->forward_transitions[actual_new_node->forward_transitions.size() - 1]);
}

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