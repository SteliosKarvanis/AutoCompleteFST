#include <limits>
#include "../src/fst.h"

#define INF std::numeric_limits<int>::max()

// Autocomplete, takes a prefix and returns all words that start with that prefix
// Usage: ./autocomplete <prefix>
// Output: Saved on bin/output_autocomplete.txt
int main(int argc, char* argv[]){
    std::string file = "../data/american-english-sorted";
    FST* fst = new FST();
    fst->buildFST(file);
    if(argc == 2){
        std::string text = argv[1];
        auto words = fst->autocomplete(text, INF);
        write_vector_to_file(words, "../output_files/output_autocomplete.txt");
    }
    else
        std::cout << "Usage: ./autocomplete <prefix>\n";
    delete fst;
    return 0;
}