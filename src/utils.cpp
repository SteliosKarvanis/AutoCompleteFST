#include "utils.h"

std::vector<std::string>read_vector_from_file(const std::string& file){
    std::ifstream myfile(file);
    std::string word;
    std::vector<std::string> words;
    // Build
    while(std::getline(myfile, word))
        words.push_back(word);
    return words;
}

void write_vector_to_file(const std::vector<std::string>& strings, const std::string& file){
    std::ofstream outfile(file);
    for(const std::string& str : strings){
        outfile << str << "\n";
    }
    outfile.flush();
    outfile.close();
}


bool is_sorted(const std::string& s1, const std::string& s2){
    int index = 0;
    while(index < s1.size() && index < s2.size()){
        int value1 = s1[index];
        int value2 = s2[index];
        if(value1 > value2)
            return false;
        else if(value1 < value2){
            return true;
        }
        index++;
    }
    // Equal so far
    if(s1.size() > s2.size())
        return false;
    else
        return true;
}

void sort_file(const std::string& input_file, const std::string& output_file){
    std::ifstream myfile(input_file);
    std::string word;
    std::vector<std::string> words;
    while(std::getline(myfile, word)){
        words.push_back(word);
    }
    std::sort(words.begin(), words.end(), is_sorted);
    write_vector_to_file(words, output_file);
}

//////////////////////////////////////////////////////
///////////////// Binary Search //////////////////////
//////////////////////////////////////////////////////

/// @brief Do a binary search finding the subset of a list of words that contains words with the given prefix, 
///         returning the index range of the words [out_min_idx, out_max_idx[
/// @param prefix: the prefix to look for in the words_list 
/// @param words_list: a list of words to fetch the prefix
/// @param out_min_idx: the bottom bound of the output set
/// @param out_max_idx: the upper bound of the output set
std::vector<std::string> binary_search(const std::string& prefix, const std::vector<std::string>& words_list){
    std::vector<std::string> output_words;
    int curr_idx = get_smaller_common_prefix_word_index(prefix, words_list, 0, words_list.size());
    while(curr_idx < words_list.size() && words_list[curr_idx].substr(0, prefix.size()) == prefix){
        output_words.push_back(words_list[curr_idx]);
        curr_idx++;
    }
    return output_words;
}

int get_smaller_common_prefix_word_index(const std::string& word, const std::vector<std::string>& words_list, int left_idx, int right_idx){
    if(left_idx == right_idx)
        return left_idx;

    int middle_idx = (left_idx + right_idx) / 2;
    if(word == words_list[middle_idx])
        return middle_idx;
    else if(is_sorted(word, words_list[middle_idx]))
        return get_smaller_common_prefix_word_index(word, words_list, left_idx, middle_idx);
    else
        return get_smaller_common_prefix_word_index(word, words_list, middle_idx + 1, right_idx);
}