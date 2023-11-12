#include <limits>
#include <chrono>
#include "../src/fst.h"

#define INF std::numeric_limits<int>::max()

void print_infos(const std::string& mode, long int memory_usage, long int build_time, long int size){
   std::cout << mode << "{\n\tmemory_usage: " << (float)memory_usage/1000 << " Kb\n\tbuild_index_time: " << build_time << "\n\tsize: " << size << "\n}" << std::endl;
}

int get_memory_usage_from_vector(std::vector<std::string> words){
    int memory_usage = 0;
    for(auto word : words)
        memory_usage += sizeof(word);
    return memory_usage;
}

int main(){
    auto files = {
        "../data/sample.txt",
        "../data/american-english-sorted",
    };
    for(auto file : files){
        std::cout << "File: " << file << std::endl;
        
        // FST
        auto start_time_fst = std::chrono::high_resolution_clock::now();
        FST* fst = new FST();
        fst->buildFST(file);
        auto end_time_fst = std::chrono::high_resolution_clock::now();
        auto duration_fst = std::chrono::duration_cast<std::chrono::microseconds>(end_time_fst - start_time_fst);
        print_infos("FST", fst->memory_usage(), duration_fst.count(), fst->count_nodes());
        
        // Binary Search
        auto start_time_bin = std::chrono::high_resolution_clock::now();
        std::vector<std::string> words = read_vector_from_file(file);
        auto end_time_bin = std::chrono::high_resolution_clock::now();
        auto duration_bin = std::chrono::duration_cast<std::chrono::microseconds>(end_time_bin - start_time_bin);
        print_infos("Binary Search", get_memory_usage_from_vector(words), duration_bin.count(), words.size());
    }

    return 0;
}