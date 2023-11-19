#include "../src/fst.h"

// Levestein, takes a prefix and returns all words that are at most dist away from the prefix
// Usage: ./levestein <prefix> <dist>
// Output: Saved on bin/output_levestein.txt
int main(int argc, char* argv[]){
    std::string file = "../american_fst.txt";
    FST* fst = new FST();
    fst->readFST(file);

    if(argc == 3){
        std::string text = argv[1];
        int dist = std::stoi(argv[2]);
        auto words = fst->levestein(text, dist);
        write_vector_to_file(words, "../output_files/output_levestein.txt");
    }
    else
        std::cout << "Usage: ./levestein <prefix> <dist>\n";
    return 0;
}
