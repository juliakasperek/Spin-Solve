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
#include <QMessageBox>

class GameController : public QWidget {
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
    bool letterDialogOpen = false;
    bool gameActive = true;

    // UI components
    QLabel *phraseLabel;
    QLabel *gemsLabel;
    QLabel *categoryLabel;
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

    // Timer management
    QTimer *gameTimer = nullptr;
    QLabel *timerLabel = nullptr;
    int remainingTime;
    QString timeText;

    // Message Boxes and Dialogs
    QList<QMessageBox*> activeMessageBoxes;
    QList<QDialog*> activeDialogs;

private:
    // Helper methods
    void updateDisplayedPhrase();
    void setUpWheel();
    void setUpUI();
    void initializePhrase();
    void paintEvent(QPaintEvent *event);
    void handleGuess();
    void updateTimer();
    void startNewGame();
    void closeAllDialogs();
    void endGame(const QString &title, const QString &message);
    void askForLetter();
    void startLetterGuessing(const QString &landedSegment);
    void showWarningAndRetry(const QString &title, const QString &text, std::function<void()> retry, bool retryDialog);
    void handleWheelReward(const QString &landedSegment);
    void handleIncorrectGuess(const QString &landedSegment);
    void updateTimerLabel();

private slots:
    void spinWheel();
    void buyVowel();
    void buyHint();
    void returnToMainMenu(bool skipConfirmation);
    void openHelpScreen();
    void solvePhrase();
};

#endif // GAMECONTROLLER_H

