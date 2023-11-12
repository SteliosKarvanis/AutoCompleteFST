#include "src/fst.h"

int main(int argc, char* argv[]){
    std::string file = argv[1];
    std::string output_file = argv[2];
    FST* fst = new FST();
    if(!fst->check_data(file))
        sort_file(file, file);
    fst->buildFST(file);
    fst->write_graph_to_file(output_file);
    return 0;
}
