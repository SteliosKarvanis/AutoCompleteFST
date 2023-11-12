#include "src/fst.h"
#include <limits>

#define INF std::numeric_limits<int>::max()

// Autocomplete, takes a prefix and returns all words that start with that prefix
// Usage: ./autocomplete <prefix>
// Ouput: Saved on bin/output_autocomplete.txt
int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: ./autocomplete <prefix>\n";
        return 1;
    }
    std::string text = argv[1];
    std::string file = "../data/american-english-sorted";
    FST* fst = new FST();
    fst->buildFST(file);
    auto words = fst->autocomplete(text, INF);
    write_vector_to_file(words, "../bin/output_autocomplete.txt");
    return 0;
}