#include <QApplication>

#include "fst.h"
#include "gui.h"
#include <memory>

int main(int argc, char* argv[]){
    std::string parser_file = "../parser.txt";
    std::ifstream myfile;
    myfile.open(parser_file);
    std::string file;
    std::getline(myfile, file);
    file = "../" + file;

    QApplication app(argc, argv);
    AutoCompleteUI* autocomplete = new AutoCompleteUI(nullptr, file);
    autocomplete->show();
    return app.exec();
}