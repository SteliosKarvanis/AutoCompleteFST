#include "utils.h"

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