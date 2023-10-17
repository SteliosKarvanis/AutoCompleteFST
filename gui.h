#ifndef TEXTADDER_H
#define TEXTADDER_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include "fst.h"


class AutoCompleteUI : public QMainWindow {
Q_OBJECT

public:
    explicit AutoCompleteUI(QWidget* parent = nullptr, std::string data_file = "");
    ~AutoCompleteUI() = default;

public slots:
    void get_predictions();

private:
    FST* fst;
    QWidget* central_widget;
    QLineEdit* input_field;
    QLabel* display_label;

    void create_actions();
    void create_layout();
    void create_widgets();
};

#endif // TEXTADDER_H
