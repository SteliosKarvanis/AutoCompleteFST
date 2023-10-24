#ifndef TEXTADDER_H
#define TEXTADDER_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QIntValidator>
#include <unordered_map>

#include <iostream>
#include "fst.h"

enum class MODE {
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    /////////// Update MODE_TO_STRING, when changing this enum ////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    AUTOCOMPLETE = 0,
    LEVENSHTEIN = 1,
    MODE_COUNT = 2,
};

std::string MODE_TO_STRING(MODE MODE);

class AutoCompleteUI : public QMainWindow {
Q_OBJECT

public:
    explicit AutoCompleteUI(QWidget* parent = nullptr, std::string data_file = "");
    ~AutoCompleteUI() = default;

public slots:
    void get_predictions();
    void change_mode(int mode_idx);
    void change_levenstein_distance();

private:
    FST* fst;
    QWidget* central_widget;
    QLineEdit* input_field;
    QLabel* display_label;
    QComboBox* mode_box;
    QLineEdit* levenstein_distance_label;

    MODE current_mode;
    int levenstein_distance;
    std::vector<std::string> result_factory(const std::string& word);

    void create_actions();
    void create_layout();
    void create_widgets();
};

#endif // TEXTADDER_H
