#include "src/fst.h"

// Levestein, takes a prefix and returns all words that are at most dist away from the prefix
// Usage: ./levestein <prefix> <dist>
// Output: Saved on bin/output_levestein.txt
int main(int argc, char* argv[]){
    if(argc != 3){
        std::cout << "Usage: ./levestein <prefix> <dist>\n";
        return 1;
    }
    std::string text = argv[1];
    int dist = std::stoi(argv[2]);

    std::string file = "../data/american-english-sorted";
    FST* fst = new FST();
    fst->buildFST(file);
    auto words = fst->levestein(text, dist);
    write_vector_to_file(words, "../output_files/output_levestein.txt");
    return 0;
}
