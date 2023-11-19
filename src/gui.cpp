#include "gui.h"

std::string MODE_TO_STRING(MODE MODE){
    switch(MODE){
        case MODE::AUTOCOMPLETE:
            return "AUTOCOMPLETE";
        case MODE::LEVENSHTEIN:
            return "LEVENSHTEIN";
        case MODE::LEVENSHTEIN_DFA:
            return "LEVENSHTEIN_DFA";
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

    this->levesteinDFA = new LevesteinDFA();

    this->fst->readFST(data_file);
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
    if(this->current_mode == MODE::LEVENSHTEIN){
        this->levenstein_distance_label->setVisible(true);
        this->levenstein_key_label->setVisible(false);
    }
    else if(this->current_mode == MODE::LEVENSHTEIN_DFA){
        this->levenstein_distance_label->setVisible(true);
        this->levenstein_key_label->setVisible(true);
        this->build_dfa();
    }
    else{
        this->levenstein_distance_label->setVisible(false);
        this->levenstein_key_label->setVisible(false);
    }
    this->get_predictions();
    this->update();
}

void AutoCompleteUI::change_levenstein_distance(){
    if(this->levenstein_distance_label->hasAcceptableInput() == false)
        return;
    if(current_mode == MODE::LEVENSHTEIN_DFA)
        this->build_dfa();
    this->get_predictions();
}

std::vector<std::string> AutoCompleteUI::result_factory(const std::string& word){
    if(this->current_mode == MODE::AUTOCOMPLETE)
        return this->fst->autocomplete(word);
    else if(this->current_mode == MODE::LEVENSHTEIN){
        int levestein_distance = this->levenstein_distance_label->text().toInt();
        return this->fst->levestein(word, levestein_distance);
    }
    else{
        bool valid = this->levesteinDFA->check(word);
        if(valid)
            return {"true"};
        return {"false"};
    }
}
void AutoCompleteUI::build_dfa(){
    std::string text = this->levenstein_key_label->text().toStdString();
    int distance = this->levenstein_distance_label->text().toInt();
    this->levesteinDFA->build(text, distance);
    this->get_predictions();
}

void AutoCompleteUI::create_actions(){
    connect(input_field, &QLineEdit::textChanged, this, &AutoCompleteUI::get_predictions);
    connect(mode_box, SIGNAL(currentIndexChanged(int)), this, SLOT(change_mode(int)));
    connect(levenstein_distance_label, &QLineEdit::textChanged, this, &AutoCompleteUI::change_levenstein_distance);
    connect(levenstein_key_label, &QLineEdit::textChanged, this, &AutoCompleteUI::build_dfa);
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

    this->levenstein_key_label = new QLineEdit(central_widget);
    this->levenstein_key_label->setVisible(false);
}

void AutoCompleteUI::create_layout(){
    QHBoxLayout* mode_layout = new QHBoxLayout();
    mode_layout->addWidget(mode_box);
    mode_layout->addWidget(levenstein_distance_label);
    mode_layout->addWidget(levenstein_key_label);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addLayout(mode_layout);
    layout->addWidget(input_field);
    layout->addWidget(display_label);
    this->central_widget->setLayout(layout);
}