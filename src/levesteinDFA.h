#include "fst.h"

class LevesteinDFA : public FST{
public:
    LevesteinDFA(std::string filename);
    void build_levestein_DFA(const std::string& word, int dist);
    bool test_DFA(Node* DFA, const std::string& word);
private:
    void levestein_dfs(std::set<Node*>& dfa_nodes, Node* actual_node, const std::string& word, std::string curr_word, const int dist, int curr_dist, int char_idx);
    void rebuild_DFA_from_nodes(Node* actual_node, const std::set<Node*>& DFA_nodes);
};