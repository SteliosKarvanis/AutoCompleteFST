#include "fst.h"

FST::FST(){
    this->root = nullptr;
    this->final_frozen_nodes = std::vector<Node*>();
}

void FST::check_data(std::string filename){
    std::ifstream myfile;
    myfile.open(filename);

    std::string word;
    std::string previous_word = "";
    while(std::getline(myfile, word)){
        // Compare strings
        if(!is_sorted(previous_word, word)){
            std::cerr << "Invalid " << previous_word << " " << word << std::endl;
            throw std::exception();
        }
        previous_word = word;
    }
}

void FST::buildFST(std::string filename){
    std::ifstream myfile;
    std::string word;
    myfile.open(filename);
    while(std::getline(myfile, word)){
        dummy_add_word(word);
    }
}

void FST::froze_node_tree(Node* node){
    if(node->frozen)
        return;
    node->frozen = true;
    if(node->valid)
        this->final_frozen_nodes.push_back(node);
    for(auto next_node : node->next_nodes){
        if(next_node->frozen)
            continue;
        froze_node_tree(next_node);
    }
}

Node* FST::get_next_node_with_last_char(Node* node){
    return node->next_nodes[node->next_nodes.size() - 1];
}

char FST::get_next_last_char(Node* node){
    return node->forward_transitions[node->forward_transitions.size() - 1]->character;
}
void FST::dummy_add_word(std::string word){
// Add a way to indentify the branch creation to frozen the previous
    int char_index = 0;
    if(this->root == nullptr)
        this->root = new Node();
    Node* actual_node = this->root;

    for(char current_char : word){
        Node* new_node;
        // if no next edge or the last next add is not the desired, create a new one and add to graph
        if(actual_node->next_nodes.empty() || get_next_last_char(actual_node) != current_char){
            new_node = new Node();
            Transition* transition = new Transition(current_char);

            actual_node->next_nodes.push_back(new_node);
            actual_node->forward_transitions.push_back(transition);

            new_node->previous_nodes.push_back(actual_node);
            new_node->backward_transitions.push_back(transition);
        }
        else{
            new_node = get_next_node_with_last_char(actual_node);
        }
        actual_node = new_node;
    }
    actual_node->valid = true;
}

bool FST::is_sorted(std::string s1, std::string s2){
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