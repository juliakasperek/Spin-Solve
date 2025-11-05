#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Instructions : public QWidget
{
    Q_OBJECT

public:
    explicit Instructions(QWidget *parent = nullptr);

signals:
    void backToMenu(); // signal to return to main menu

private:
    QPushButton *backButton;
    QLabel *instructionsLabel;
};

#endif // INSTRUCTIONS_H
