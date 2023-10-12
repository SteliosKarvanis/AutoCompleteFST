#include "gui.h"
#include <iostream>

AutoCompleteUI::AutoCompleteUI(QWidget* parent) : QMainWindow(parent){
    setWindowTitle("AutoComplete");

    this->central_widget = nullptr;
    this->display_label = nullptr;
    this->input_field = nullptr;

    this->create_widgets();
    this->create_layout();
    this->create_actions();

    setCentralWidget(central_widget);
}

void AutoCompleteUI::add_text(){
    QString text = this->input_field->text();
    this->display_label->setText(text);
}

void AutoCompleteUI::create_actions(){
    connect(input_field, &QLineEdit::textChanged, this, &AutoCompleteUI::add_text);
}

void AutoCompleteUI::create_widgets(){
    this->central_widget = new QWidget(this);
    this->display_label = new QLabel(central_widget);
    this->display_label->setWordWrap(true);
    this->input_field = new QLineEdit(central_widget);

}

void AutoCompleteUI::create_layout(){
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(input_field);
    layout->addWidget(display_label);
    this->central_widget->setLayout(layout);  // Set the layout for the central_widget
}