#include <QApplication>

#include "fst.h"
#include "gui.h"
#include <memory>

#define shp std::shared_ptr

// int main(int argc, char* argv[]) {
//     QApplication app(argc, argv);
//     AutoCompleteUI* autocomplete = new AutoCompleteUI(nullptr);
//     autocomplete->show();
//     return app.exec();
// }


int main(){
    std::string file = "../data/american-english-sorted";
    shp<FST> fst = shp<FST>();
    fst->check_data(file);
    std::cout << "Data valid!!!" << std::endl;
    return 0;
}