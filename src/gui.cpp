#include "gui.h"

AutoCompleteUI::AutoCompleteUI(QWidget* parent, std::string data_file) : QMainWindow(parent){
    setWindowTitle("AutoComplete");

    this->central_widget = nullptr;
    this->display_label = nullptr;
    this->input_field = nullptr;
    this->fst = new FST();

    this->fst->buildFST(data_file);
    this->create_widgets();
    this->create_layout();
    this->create_actions();

    setCentralWidget(central_widget);
}

void AutoCompleteUI::get_predictions(){
    std::string input_text = this->input_field->text().toStdString();
    auto words = fst->retrieve_words(input_text);
    std::string output_text;
    for(auto word : words){
        output_text = output_text + word;
        output_text.push_back('\n');
    }
    this->display_label->setText(QString::fromStdString(output_text));
    this->display_label->adjustSize();
    this->adjustSize();
}

void AutoCompleteUI::create_actions(){
    connect(input_field, &QLineEdit::textChanged, this, &AutoCompleteUI::get_predictions);
}

void AutoCompleteUI::create_widgets(){
    this->central_widget = new QWidget(this);
    this->display_label = new QLabel(central_widget);
    this->display_label->setAlignment(Qt::AlignTop);
    this->display_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->display_label->setWordWrap(true);
    this->input_field = new QLineEdit(central_widget);

}

void AutoCompleteUI::create_layout(){
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(input_field);
    layout->addWidget(display_label);
    this->central_widget->setLayout(layout);
}