#include <iostream>
#include "src/utils.h"

// Binary Search, takes a prefix and returns all words that start with that prefix
// Usage: ./binary_search <prefix>
// Output: Saved on bin/output_bin_search.txt
int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: ./binary_search <prefix>\n";
        return 1;
    }
    std::string data_file = "../data/american-english-sorted";
    std::string text = argv[1];
    // Read Data 
    auto words = read_vector_from_file(data_file);

    std::vector<std::string> output_words = binary_search(text, words);
    write_vector_to_file(output_words, "../output_files/output_bin_search.txt");
    return 0;
}