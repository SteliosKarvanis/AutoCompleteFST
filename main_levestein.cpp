#include "fst.h"

int main(int argc, char* argv[]){
    std::string text = argv[1];
    int dist = std::stoi(argv[2]);

    std::string file = "../data/american-english-sorted";
    FST* fst = new FST();
    fst->buildFST(file);
    auto words = fst->levestein(text, dist);
    write_vector_to_file(words, "../bin/output_levestein.txt");
    return 0;
}
