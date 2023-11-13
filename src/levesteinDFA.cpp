#include "levesteinDFA.h"

LevesteinDFA::LevesteinDFA(std::string filename) : FST(){
    this->buildFST(filename);
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// SEARCH UTILS ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void LevesteinDFA::build_levestein_DFA(const std::string& word, int dist){
    std::set<Node*> DFA_nodes;
    this->levestein_dfs(DFA_nodes, this->root, word, "", dist, 0, 0);
    this->rebuild_DFA_from_nodes(root, DFA_nodes);
}

void LevesteinDFA::levestein_dfs(std::set<Node*>& dfa_nodes, Node* actual_node, const std::string& word, std::string curr_word, const int dist, int curr_dist, int char_idx){
    if(curr_dist > dist)
        return;
    dfa_nodes.insert(actual_node);

    for(int i = 0; i < actual_node->next_nodes.size(); i++){
        char transition = actual_node->forward_transitions[i];
        auto next_node = actual_node->next_nodes[i];
        int new_dist_in_addition;
        // Each follow recursion, cover a different action when modificating the original word
        if(char_idx < word.size() && word[char_idx] == transition){
            // Match (Follow a transition, making a char match)
            this->levestein_dfs(dfa_nodes, next_node, word, curr_word + transition, dist, curr_dist, char_idx + 1);
        }
        else{
            // Replace (Follow a transition, going to the next char to be analised)
            this->levestein_dfs(dfa_nodes, next_node, word, curr_word + transition, dist, curr_dist + 1, char_idx + 1);
            // Addition (Follow a transition, remaining the char to be analised)
            this->levestein_dfs(dfa_nodes, next_node, word, curr_word + transition, dist, curr_dist + 1, char_idx);
            // Deleting (Stay in the state, skipping the current char to be analysed)
            this->levestein_dfs(dfa_nodes, actual_node, word, curr_word, dist, curr_dist + 1, char_idx + 1);
        }
    }
}

void LevesteinDFA::rebuild_DFA_from_nodes(Node* actual_node, const std::set<Node*>& DFA_nodes){
    for(int i = actual_node->next_nodes.size() - 1; i >= 0; i--){
        if(DFA_nodes.find(actual_node->next_nodes[i]) == DFA_nodes.end()){
            actual_node->next_nodes.erase(actual_node->next_nodes.begin() + i);
        }
        else{
            this->rebuild_DFA_from_nodes(actual_node->next_nodes[i], DFA_nodes);
        }
    }
}

bool LevesteinDFA::test_DFA(Node* DFA, const std::string& word){
    auto prefix_node = this->retrieve_node_with_prefix(word);
    if(prefix_node == nullptr)
        return false;
    return prefix_node->valid;
}
