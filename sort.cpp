// #include <vector>
// #include <fstream>
// #include <algorithm>
#include <string>

bool is_sorted(std::string s1, std::string s2){
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

// int main(){
//     std::string file = "data/american-english";
//     std::ifstream myfile; 
//     myfile.open(file);
//     std::string word;
//     std::vector<std::string> words;
//     while(std::getline(myfile, word)){
//         words.push_back(word);
//     }
//     std::sort(words.begin(), words.end(), is_sorted);
//     std::ofstream output_file;
//     output_file.open("data/american-english-sorted");
//     for(auto word : words){
//         output_file << word << std::endl;
//     }
//     return 0;
// }
