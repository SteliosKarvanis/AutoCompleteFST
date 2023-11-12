#include "src/utils.h"

int main(int argc, char* argv[]){
    std::string data_file = "../data/american-english-sorted";
    std::string text = argv[1];
    // Read Data 
    auto words = read_vector_from_file(data_file);
    
    std::vector<std::string> output_words = binary_search(text, words);
    write_vector_to_file(output_words, "../bin/output_bin_search.txt");
    return 0;
}