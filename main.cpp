#include <QApplication>

#include "fst.h"
#include "gui.h"
#include <memory>

#define shp std::shared_ptr

// int main(int argc, char* argv[]) {
//     std::string parser_file = "../parser.txt";
//     std::ifstream myfile;
//     myfile.open(parser_file);
//     std::string file;
//     std::getline(myfile, file);
//     file = "../" + file;

//     QApplication app(argc, argv);
//     AutoCompleteUI* autocomplete = new AutoCompleteUI(nullptr, file);
//     autocomplete->show();
//     return app.exec();
// }


int main(){
    std::string file = "../data/sample.txt";
    FST *fst = new FST();
    if(!fst->check_data(file))
        std::cout << "Data not sorted!!" << std::endl;
    fst->buildFST(file);
    std::string text;
    std::cin >> text;
    auto words = fst->retrieve_words(text);
    for(auto word : words){
        std::cout << word << std::endl;
    }
    return 0;
}