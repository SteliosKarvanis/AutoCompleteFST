#include "src/utils.h"

int main(int argc, char* argv[]){
    std::string data_file = "../data/american-english-sorted";
    std::string text = argv[1];
    // Read Data 
    auto words = read_vector_from_file(data_file);
    
    int min_idx, max_idx;
    binary_search(text, words, min_idx, max_idx);
    std::vector<std::string> output_words;
    for(int i = min_idx; i < max_idx; i++){
        output_words.push_back(words[i]);
    }
    write_vector_to_file(output_words, "../bin/output_bin_search.txt");
    return 0;
}