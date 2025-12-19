#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Wheel.h"
#include "PlayerGems.h"
#include "PhraseHandler.h"
#include "PhraseLibrary.h"  // Needed to access Phrase class

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

//Testing
    Wheel* getWheel() const { return wheel; }
    QLabel* getPhraseLabel() const { return phraseLabel; }
    QLabel* getCategoryLabel() const { return categoryLabel; }
    QLabel* getTimerLabel() const { return timerLabel; }
    QPushButton* getSpinButton() const { return spinButton; }
    QPushButton* getBuyVowelButton() const { return buyVowelButton; }
    QPushButton* getBuyHintButton() const { return buyHintButton; }
    QPushButton* getSolveButton() const { return solveButton; }

    void setWheel(Wheel* newWheel) {
        if (wheel) delete wheel;  // optional: clean up old wheel
        wheel = newWheel;
    }
    void testSpinWheel();

private:
    // Game state
    int difficulty;
    int freeHintsCount = 0;
    QString phrase;
    QString displayedPhrase;
    QSet<QChar> guessedLetters;
    bool bypassCloseConfirm = false;
    bool letterDialogOpen = false;
    bool gameActive = true;

    PhraseHandler *phraseHandler = nullptr;
    Wheel *wheel = nullptr;
    PlayerGems playerGems;

    // UI elements
    QLabel *phraseLabel = nullptr;
    QLabel *gemsLabel = nullptr;
    QLabel *categoryLabel = nullptr;
    QLabel *wheelResultLabel = nullptr;
    QLabel *freeHintsLabel = nullptr;
    QLabel *timerLabel = nullptr;

    QLineEdit *guessedLettersBox = nullptr;

    QPushButton *spinButton = nullptr;
    QPushButton *buyVowelButton = nullptr;
    QPushButton *buyHintButton = nullptr;
    QPushButton *mainMenuButton = nullptr;
    QPushButton *helpButton = nullptr;
    QPushButton *solveButton = nullptr;

    // Timer management
    QTimer *gameTimer = nullptr;
    int remainingTime;
    QString timeText;

    // Dialog Tracking
    QList<QMessageBox*> activeMessageBoxes;
    QList<QDialog*> activeDialogs;

private:
    // Setup/ Initialization
    void setUpWheel();
    void setUpUI();
    void initializePhrase();
    void setUpLabels();

    // Rendering
    void paintEvent(QPaintEvent *event);

    // Game Logic
    void updateDisplayedPhrase();
    void updateTimer();
    void updateTimerLabel();
    void handleGuess();
    void startNewGame();
    void endGame(const QString &title, const QString &message);
    void askForLetter();
    void startLetterGuessing(const QString &landedSegment);
    void showWarningAndRetry(const QString &title, const QString &text, std::function<void()> retry, bool retryDialog);
    void handleWheelReward(const QString &landedSegment);
    void handleIncorrectGuess(const QString &landedSegment);
    void closeAllDialogs();

    int currentHintIndex = 0;
    QStringList hintsForCurrentPhrase;

private slots:
    // Slots
    void spinWheel();
    void buyVowel();
    void buyHint();
    void solvePhrase();
    void openHelpScreen();
    void returnToMainMenu(bool skipConfirmation);
};

#endif // GAMECONTROLLER_H

