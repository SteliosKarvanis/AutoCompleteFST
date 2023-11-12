#include "src/fst.h"
#include "src/utils.h"
#include <fstream>
#include <chrono>
#include <limits>

#define INF std::numeric_limits<int>::max()

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: ./time_counter <input_file>\n";
        return 1;
    }
    std::string input_file = argv[1];
    std::string file = "../data/american-english-sorted";
    FST* fst = new FST();
    fst->buildFST(file);
    auto words_list = read_vector_from_file(file);
    auto input_examples = read_vector_from_file(input_file);
    std::ofstream out_file("../times.txt");
    for(auto example : input_examples){
        // FST
        auto start_time_fst = std::chrono::high_resolution_clock::now();
        auto words_fst = fst->autocomplete(example, INF);
        auto end_time_fst = std::chrono::high_resolution_clock::now();
        auto duration_fst = std::chrono::duration_cast<std::chrono::microseconds>(end_time_fst - start_time_fst);

        // Bin Search
        auto start_time_bin_search = std::chrono::high_resolution_clock::now();
        auto words_bin_search = binary_search(example, words_list);
        auto end_time_bin_search = std::chrono::high_resolution_clock::now();
        auto duration_bin_search = std::chrono::duration_cast<std::chrono::microseconds>(end_time_bin_search - start_time_bin_search);

        // Write to file
        out_file << example << " " << std::to_string(duration_fst.count()) << " " << std::to_string(duration_bin_search.count()) << "\n";
    }
    out_file.close();
    return 0;
}
