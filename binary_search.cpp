#include <vector>
#include <fstream>
#include <iostream>
#include "src/utils.h"


int get_smaller_common_prefix_word_index(const std::string& word, const std::vector<std::string>& words_list, int left_idx, int right_idx){
    if(left_idx == right_idx)
        return left_idx;

    int middle_idx = (left_idx + right_idx)/2;
    if(word == words_list[middle_idx])
        return middle_idx;
    else if(is_sorted(word, words_list[middle_idx]))
        return get_smaller_common_prefix_word_index(word, words_list, left_idx, middle_idx);
    else
        return get_smaller_common_prefix_word_index(word, words_list, middle_idx + 1, right_idx);
}

/// @brief Do a binary search finding the subset of a list of words that contains words with the given prefix, 
///         returning the index range of the words [out_min_idx, out_max_idx[
/// @param prefix: the prefix to look for in the words_list 
/// @param words_list: a list of words to fetch the prefix
/// @param out_min_idx: the bottom bound of the output set
/// @param out_max_idx: the upper bound of the output set
void binary_search(const std::string& prefix, const std::vector<std::string>& words_list, int& out_min_idx, int& out_max_idx){
    int curr_idx = get_smaller_common_prefix_word_index(prefix, words_list, 0, words_list.size());
    out_min_idx = curr_idx;
    while(curr_idx < words_list.size() && words_list[curr_idx].substr(0, prefix.size()) == prefix)
        curr_idx++;
    out_max_idx = curr_idx;
}

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