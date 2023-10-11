#include <QApplication>
#include "gui.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    AutoCompleteUI* autocomplete = new AutoCompleteUI(nullptr);
    autocomplete->show();
    return app.exec();
}
