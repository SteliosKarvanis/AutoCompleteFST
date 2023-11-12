#include "gui.h"

std::string MODE_TO_STRING(MODE MODE) {
    switch (MODE) {
        case MODE::AUTOCOMPLETE:
            return "AUTOCOMPLETE";
        case MODE::LEVENSHTEIN:
            return "LEVENSHTEIN";
        default:
            return "NONE";
    }
}

AutoCompleteUI::AutoCompleteUI(QWidget* parent, std::string data_file) : QMainWindow(parent){
    setWindowTitle("AutoComplete");

    this->central_widget = nullptr;
    this->display_label = nullptr;
    this->input_field = nullptr;
    this->mode_box = nullptr;
    this->levenstein_distance_label = nullptr;

    this->fst = new FST();
    this->current_mode = MODE::AUTOCOMPLETE;
    this->levenstein_distance = 0;

    this->fst->buildFST(data_file);
    this->create_widgets();
    this->create_layout();
    this->create_actions();

    setCentralWidget(central_widget);
}

void AutoCompleteUI::get_predictions(){
    std::string input_text = this->input_field->text().toStdString();
    auto words = this->result_factory(input_text);
    std::string output_text;
    for(auto word : words){
        output_text = output_text + word;
        output_text.push_back('\n');
    }
    this->display_label->setText(QString::fromStdString(output_text));
    this->display_label->adjustSize();
    this->adjustSize();
}

void AutoCompleteUI::change_mode(int mode_idx){
    this->current_mode = static_cast<MODE>(mode_idx);
    if(this->current_mode == MODE::LEVENSHTEIN)
        this->levenstein_distance_label->setVisible(true);
    else
        this->levenstein_distance_label->setVisible(false);
    this->update();
}

void AutoCompleteUI::change_levenstein_distance(){
    if(this->levenstein_distance_label->hasAcceptableInput() == false)
        return;

    std::string input_text = this->levenstein_distance_label->text().toStdString();
    this->levenstein_distance = std::stoi(input_text);
}

std::vector<std::string> AutoCompleteUI::result_factory(const std::string& word){
    if(this->current_mode == MODE::AUTOCOMPLETE)
        return this->fst->autocomplete(word);
    else
        return this->fst->levestein(word, this->levenstein_distance);
}

void AutoCompleteUI::create_actions(){
    connect(input_field, &QLineEdit::textChanged, this, &AutoCompleteUI::get_predictions);
    connect(mode_box, SIGNAL(currentIndexChanged(int)), this, SLOT(change_mode(int)));
    connect(levenstein_distance_label, &QLineEdit::textChanged, this, &AutoCompleteUI::change_levenstein_distance);
}

void AutoCompleteUI::create_widgets(){
    this->central_widget = new QWidget(this);
    this->display_label = new QLabel(central_widget);
    this->input_field = new QLineEdit(central_widget);
    
    this->display_label->setAlignment(Qt::AlignTop);
    this->display_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->display_label->setWordWrap(true);
    
    this->mode_box = new QComboBox(central_widget);
    for(int i = 0; i < static_cast<int>(MODE::MODE_COUNT); i++){
        this->mode_box->addItem(QString::fromStdString(MODE_TO_STRING(static_cast<MODE>(i))));
    }

    this->levenstein_distance_label = new QLineEdit(central_widget);
    this->levenstein_distance_label->setText(QString::fromStdString("0"));
    this->levenstein_distance_label->setVisible(false);
    this->levenstein_distance_label->setValidator(new QIntValidator(0, INT_MAX));
}

void AutoCompleteUI::create_layout(){
    QHBoxLayout* mode_layout = new QHBoxLayout();
    mode_layout->addWidget(mode_box);
    mode_layout->addWidget(levenstein_distance_label);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addLayout(mode_layout);
    layout->addWidget(input_field);
    layout->addWidget(display_label);
    this->central_widget->setLayout(layout);
}