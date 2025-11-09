#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Wheel.h"
#include "PlayerGems.h"
#include "PhraseHandler.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSet>

class GameController : public QWidget
{
    Q_OBJECT

public:
    explicit GameController(int diff, QWidget *parent = nullptr);

private:
    // Game state
    int difficulty;
    QString phrase;
    QString displayedPhrase;
    QSet<QChar> guessedLetters;
    PhraseHandler *phraseHandler = nullptr;
    bool bypassCloseConfirm = false;


    // UI components
    QLabel *phraseLabel;
    QLabel *gemsLabel;
    QLabel *wheelResultLabel;
    QLineEdit *guessedLettersBox;
    QPushButton *spinButton;
    QPushButton *buyVowelButton;
    QPushButton *buyHintButton;
    QPushButton *mainMenuButton;
    QPushButton *helpButton;
    QPushButton *solveButton;

    // Wheel and gems management
    Wheel *wheel = nullptr;
    PlayerGems playerGems;
    QLabel *freeHintsLabel;
    int freeHintsCount = 0;

private:
    // Helper methods
    void updateDisplayedPhrase();
    void setUpWheel();
    void setUpUI();
    void initializePhrase();
    void paintEvent(QPaintEvent *event);
    void handleGuess();

private slots:
    void spinWheel();
    void buyVowel();
    void buyHint();
    void returnToMainMenu();
    void openHelpScreen();
    void solvePhrase();
};

#endif // GAMECONTROLLER_H

