#include <limits>
#include "../src/fst.h"

#define INF std::numeric_limits<int>::max()

// Autocomplete, takes a prefix and returns all words that start with that prefix
// Usage: ./autocomplete <input_file>
// Output: Saved on bin/output_autocomplete.txt
int main(int argc, char* argv[]){
    std::string file = "../data/american_english_sorted.txt";
    FST* fst = new FST();
    fst->buildFST(file);
    if(argc == 2){
        std::string input_file = argv[1];
        auto inputs = read_vector_from_file(input_file);
        for(auto text : inputs){
            auto words = fst->autocomplete(text, INF);
            words.push_back("]\n");
            write_vector_to_file(words, "../output_files/output_autocomplete.txt", true);
        }
    }
    else
        std::cout << "Usage: ./autocomplete <input_file>\n";
    delete fst;
    return 0;
}