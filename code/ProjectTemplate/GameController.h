#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSet>
#include "Wheel.h"
#include "PlayerGems.h"
#include "PhraseHandler.h"

class GameController : public QWidget
{
    Q_OBJECT

public:
    explicit GameController(int diff, QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    // Game state
    int difficulty;
    QString displayedPhrase;
    QSet<QChar> guessedLetters;
    PhraseHandler *phraseHandler = nullptr;
    bool bypassCloseConfirm = false;

    // UI components
    QLabel *phraseLabel;
    QLabel *gemsLabel;
    QLineEdit *guessedLettersBox;
    QPushButton *spinButton;
    QPushButton *buyVowelButton;
    QPushButton *buyHintButton;
    QPushButton *mainMenuButton;
    QPushButton *helpButton;
    QPushButton *solveButton;

    // Wheel, gems, and free hint management
    Wheel *wheel = nullptr;
    PlayerGems playerGems;
    QLabel *freeHintsLabel;
    int freeHintsCount = 0;

    // Helper methods
    void updateDisplayedPhrase();
    void setUpWheel();
    void setUpUI();
    void initializePhrase();

private slots:
    void spinWheel();
    void buyVowel();
    void buyHint();
    void solvePhrase();
};

#endif // GAMECONTROLLER_H
