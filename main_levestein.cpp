#include "fst.h"
#include <limits>

// void write_vector_to_file(const std::vector<std::string>& strings){
//     std::string file_path = "output.txt";

//     std::ofstream outfile(file_path);
//     for(const std::string& str : strings){
//         outfile << str << "\n";
//     }
//     outfile.flush();
//     outfile.close();
// }

int main(int argc, char* argv[]){
    std::string text;
    int dist;
    std::cin >> text;
    std::cin >> dist;
    std::string file = "../data/american-english-sorted";
    FST* fst = new FST();
    if(!fst->check_data(file))
        std::cout << "Data not sorted!!" << std::endl;
    fst->buildFST(file);
    auto words = fst->levestein(text, dist);
    std::cout << text << " " << dist << std::endl;
    for(auto word : words){
        std::cout << word << std::endl;
    }
    return 0;
}
