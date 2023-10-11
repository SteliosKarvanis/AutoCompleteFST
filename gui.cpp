#include "gui.h"
#include <iostream>

AutoCompleteUI::AutoCompleteUI(QWidget* parent) : QMainWindow(parent){
    setWindowTitle("AutoComplete");

    this->centralWidget = nullptr;
    this->displayLabel = nullptr;
    this->inputField = nullptr;

    this->create_widgets();
    this->create_layout();
    this->create_actions();

    setCentralWidget(centralWidget);
}

void AutoCompleteUI::add_text(){
    QString text = this->inputField->text();
    this->displayLabel->setText(text);
}

void AutoCompleteUI::create_actions(){
    connect(inputField, &QLineEdit::textChanged, this, &AutoCompleteUI::add_text);
}

void AutoCompleteUI::create_widgets(){
    this->centralWidget = new QWidget(this);
    this->displayLabel = new QLabel(centralWidget);
    this->displayLabel->setWordWrap(true);
    this->inputField = new QLineEdit(centralWidget);

}

void AutoCompleteUI::create_layout(){
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(inputField);
    layout->addWidget(displayLabel);
    centralWidget->setLayout(layout);  // Set the layout for the centralWidget
}