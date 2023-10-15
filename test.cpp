#include "fst.h"
#include <memory>

void write_to_file(std::vector<std::string> strings){
    std::string file_path = "output.txt";

    std::ofstream outfile(file_path);
    for(const std::string& str : strings){
        outfile << str << "\n";
    }
    outfile.flush();
    outfile.close();
}

int main(int argc, char* argv[]){
    std::string text = argv[1];
    std::string file = "../data/american-english-sorted";
    FST* fst = new FST();
    if(!fst->check_data(file))
        std::cout << "Data not sorted!!" << std::endl;
    fst->buildFST(file);
    std::vector<std::string> words = fst->retrieve_words(text);
    write_to_file(words);

    return 0;
}
