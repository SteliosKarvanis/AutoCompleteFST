#ifndef TEXTADDER_H
#define TEXTADDER_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QWidget>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QtGui>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>


class AutoCompleteUI : public QMainWindow {
Q_OBJECT

public:
    explicit AutoCompleteUI(QWidget* parent = nullptr);
    ~AutoCompleteUI() = default;
public slots:
    void add_text();

private:
    QWidget* centralWidget;
    QLineEdit* inputField;
    QLabel* displayLabel;

    void create_actions();
    void create_layout();
    void create_widgets();
};

#endif // TEXTADDER_H
