#include <QApplication>
#include "../src/gui.h"

// Main application, a GUI that allows the user to do autocomplete and levestein interatively
// Usage: ./app
int main(int argc, char* argv[]){
    std::string data_file = "../data/american_english_sorted.txt";

    QApplication app(argc, argv);
    AutoCompleteUI* autocomplete = new AutoCompleteUI(nullptr, data_file);
    autocomplete->show();
    return app.exec();
}