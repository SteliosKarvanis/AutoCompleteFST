#include "fst.h"
#include <limits>

#define INF std::numeric_limits<int>::max()

void write_vector_to_file(const std::vector<std::string>& strings){
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
    auto words = fst->retrieve_words(text, INF);
    write_vector_to_file(words);

    return 0;
}
