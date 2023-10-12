#include <QApplication>

#include "fst.h"
#include "gui.h"

// int main(int argc, char* argv[]) {
//     QApplication app(argc, argv);
//     AutoCompleteUI* autocomplete = new AutoCompleteUI(nullptr);
//     autocomplete->show();
//     return app.exec();
// }


int main(){
    std::string file = "../data/american-english-sorted";
    FST *fst = new FST();
    fst->check_data(file);
    std::cout << "Data valid!!!" << std::endl;
    return 0;
}