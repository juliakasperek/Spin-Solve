#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QCloseEvent>
#include <QSet>

class GameController : public QWidget {
    Q_OBJECT

public:
    explicit GameController(int difficulty, QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override; // handle X button to exit game and return to main screen

private slots:
    void spinWheel();
    void buyVowel();
    void buyHint();

private:
    int difficulty;  // 0 = easy, 1 = hard
    int gems;
    QString phrase;
    QString displayedPhrase;

    QLabel *phraseLabel;
    QLineEdit *guessedLettersBox;
    QPushButton *spinButton;
    QPushButton *buyVowelButton;
    QPushButton *buyHintButton;

    int currentSegment;  // segment landed on
    QSet<QChar> guessedLetters; // track guessed letters

    void updateDisplayedPhrase();
};

#endif //GAMECONTROLLER_H

