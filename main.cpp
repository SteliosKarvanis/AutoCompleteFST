#include <QApplication>
#include "src/gui.h"

int main(int argc, char* argv[]){
    std::string data_file = "../data/american-english-sorted";

    QApplication app(argc, argv);
    AutoCompleteUI* autocomplete = new AutoCompleteUI(nullptr, data_file);
    autocomplete->show();
    return app.exec();
}