#include "../src/fst.h"

// Write FST, takes a file and writes the FST to a file
// Usage: ./write_fst <input_dict_file> <output_file>
// Output: Saved on bin/output_fst.txt
int main(int argc, char* argv[]){
    if(argc != 3){
        std::cout << "Usage: ./write_fst <input_dict_file> <output_file>\n";
        return 1;
    }
    std::string file = argv[1];
    std::string output_file = argv[2];
    FST* fst = new FST();
    if(!check_data(file))
        sort_file(file, file);
    fst->buildFST(file);
    fst->write_to_file(output_file);
    return 0;
}
