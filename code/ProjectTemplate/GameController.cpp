#include "GameController.h"
#include "MainController.h"
#include "PhraseLibrary.h"
#include "PlayerGems.h"
#include "Help.h"
#include "PhraseHandler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>
#include <QPainter>
#include <QInputDialog>
#include <QTimer>

GameController::GameController(int diff, QWidget *parent)
    : QWidget(parent), difficulty(diff), playerGems(0, this)
{
    categoryLabel = new QLabel("Category: ", this);
    categoryLabel->setAlignment(Qt::AlignCenter);
    categoryLabel->setStyleSheet("font-size: 18px; color: #8F0774; font-weight: bold;");

    initializePhrase();
    setUpUI();
}

void GameController::setUpWheel() {

    if (!wheel) {
        wheel = new Wheel(this);
        wheel->setFixedSize(280, 280);
        wheel->setStyleSheet(
            "border: 5px solid #FFE3F8;"
            "border-radius: 200px;"
            "background-color: #FFE3F8;"
            );

        connect(wheel, &Wheel::landedSegment, this, [=](int index){
            QStringList segments = { "2 gems","-5 seconds","3 gems","Free Hint",
                                    "1 gem","-10 seconds","2 gems","4 gems" };
            QString landedSegment = segments[index];

            wheelResultLabel->setText("Wheel landed on: " + landedSegment);

            std::function<void()> askForLetter;
            askForLetter = [=, &askForLetter]() {

                if (!gameActive) return;   // <-- add this
                if (letterDialogOpen) return;
                letterDialogOpen = true;

                if (remainingTime <= 0) return;

                QInputDialog *dialog = new QInputDialog(this);
                dialog->setWindowTitle("Guess a Letter");
                dialog->setLabelText("Enter a letter (non-vowel only):");
                dialog->setTextValue("");
                dialog->setInputMode(QInputDialog::TextInput);
                dialog->setModal(true);

                activeDialogs.append(dialog);

                connect(dialog, &QInputDialog::textValueSelected, this, [=](const QString &guess){
                    dialog->deleteLater();
                    activeDialogs.removeOne(dialog);

                    letterDialogOpen = false;

                    if (guess.isEmpty() || guess.length() != 1 || !guess[0].isLetter()) {
                        QMessageBox *msg = new QMessageBox(this);
                        msg->setIcon(QMessageBox::Warning);
                        msg->setWindowTitle("Invalid Input");
                        msg->setText("Enter a single letter (A-Z).");

                        activeDialogs.append(msg);

                        connect(msg, &QDialog::finished, this, [=](){
                            activeDialogs.removeOne(msg);
                            msg->deleteLater();
                            QTimer::singleShot(0, this, askForLetter);
                        });
                        msg->open();
                        return;
                    }

                    QChar letter = guess[0].toUpper();

                    if (QString("AEIOU").contains(letter)) {
                        QMessageBox *msg = new QMessageBox(this);
                        msg->setIcon(QMessageBox::Information);
                        msg->setWindowTitle("Invalid Letter");
                        msg->setText("Vowels are not allowed!");

                        activeDialogs.append(msg);

                        connect(msg, &QDialog::finished, this, [=](){
                            activeDialogs.removeOne(msg);
                            msg->deleteLater();

                            QTimer::singleShot(0, this, askForLetter);
                        });
                        msg->open();
                        return;
                    }

                    if (guessedLetters.contains(letter)) {
                        QMessageBox *msg = new QMessageBox(this);
                        msg->setIcon(QMessageBox::Information);
                        msg->setWindowTitle("Already Guessed");
                        msg->setText("You already guessed that letter!");

                        activeDialogs.append(msg);

                        connect(msg, &QDialog::finished, this, [=](){
                            activeDialogs.removeOne(msg);
                            msg->deleteLater();

                            QTimer::singleShot(0, this, askForLetter);
                        });
                        msg->open();
                        return;
                    }

                    guessedLetters.insert(letter);
                    QString lettersText = "Guessed Letters: ";
                    for (auto l : guessedLetters)
                        lettersText += QString(l) + " ";
                    guessedLettersBox->setText(lettersText);

                    if (phraseHandler->guessLetter(letter)) {
                        updateDisplayedPhrase();

                        if (landedSegment.contains("gem")) {
                            int gemAmount = landedSegment.split(" ")[0].toInt();
                            playerGems.addGems(gemAmount);
                        }

                        if (landedSegment == "Free Hint") {
                            freeHintsCount++;
                            freeHintsLabel->setText("Free Hints: " + QString::number(freeHintsCount));
                        }

                        if (phraseHandler->isComplete()) {

                            gameTimer->stop();

                            QMessageBox *msg = new QMessageBox(this);
                            msg->information(this, "You Win!", "You guessed the full phrase!");
                            activeDialogs.append(msg);
                            connect(msg, &QDialog::finished, this, [=](){
                                activeDialogs.removeOne(msg);
                                msg->deleteLater();
                            });
                        }

                    } else {
                        // Incorrect guess
                        if (landedSegment == "-5 seconds") {
                            remainingTime -= 5;
                        } else if (landedSegment == "-10 seconds") {
                            remainingTime -= 10;
                        } else {
                            QMessageBox *msg = new QMessageBox(this);
                            msg->setIcon(QMessageBox::Information);
                            msg->setWindowTitle("Incorrect");
                            msg->setText("The letter is not in the phrase.");

                            activeDialogs.append(msg);

                            connect(msg, &QDialog::finished, this, [=](){
                                activeDialogs.removeOne(msg);
                                msg->deleteLater();
                            });
                            msg->open();
                            return;
                        }

                        if (remainingTime < 0) remainingTime = 0;

                        int minutes = remainingTime / 60;
                        int seconds = remainingTime % 60;
                        timerLabel->setText(QString("Time: %1:%2")
                                                .arg(minutes)
                                                .arg(seconds, 2, 10, QChar('0')));

                        if (remainingTime == 0) {
                            if (wheel) wheel->stopSpin();
                            endGame("Time's Up!", "You ran out of time! Do you want to start a new game?");
                            return;
                        }
                    }
                });

                dialog->open();

            };

            if (remainingTime <= 0) {
                return;
            }

            askForLetter();
        });
    }
};

void GameController::initializePhrase()
{
    PhraseLibrary library;

    Phrase selectedPhrase = library.getRandomPhrase(difficulty == 0 ? "easy" : "hard");

    QString phrase = QString::fromStdString(selectedPhrase.text);
    QString category = QString::fromStdString(selectedPhrase.category);

    if (categoryLabel) {
        categoryLabel->setText("Category: " + category);
    }

    if (phraseHandler) {
        delete phraseHandler;
        phraseHandler = nullptr;
    }

    phraseHandler = new PhraseHandler(phrase);
    displayedPhrase = phraseHandler->getDisplayedPhrase();
}

void GameController::updateDisplayedPhrase()
{
    if (!phraseLabel || !phraseHandler) return;

    displayedPhrase = phraseHandler->getDisplayedPhrase();

    // Display using monospace font and avoid HTML collapsing spaces
    phraseLabel->setText(displayedPhrase);
    phraseLabel->setStyleSheet("font-family: monospace; font-size: 30px; font-weight: bold;");
}

void GameController::updateTimer() {

    if (remainingTime <= 0) {
        gameTimer->stop();
        closeAllDialogs();

        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Time's Up!",
            "You ran out of time! Do you want to start a new game?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            startNewGame();
        } else {
            returnToMainMenu(false);
        }

        return;
    }

    remainingTime--;

    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    timerLabel->setText(QString("Time: %1:%2")
                            .arg(minutes)
                            .arg(seconds, 2, 10, QChar('0')));
}

void GameController::setUpUI() {
    // Phrase label
    phraseLabel = new QLabel(displayedPhrase, this);
    phraseLabel->setAlignment(Qt::AlignCenter);
    phraseLabel->setWordWrap(true);
    phraseLabel->setStyleSheet("font-size: 30px; font-weight: bold;");

    // Guessed letters box
    guessedLettersBox = new QLineEdit(this);
    guessedLettersBox->setReadOnly(true);
    guessedLettersBox->setAlignment(Qt::AlignCenter);
    guessedLettersBox->setStyleSheet("font-size: 18px;");
    guessedLettersBox->setFixedWidth(450);

    // Gems label
    gemsLabel = new QLabel("💎 Gems: " + QString::number(playerGems.getGems()), this);
    gemsLabel->setAlignment(Qt::AlignCenter);
    gemsLabel->setFixedWidth(100);
    gemsLabel->setStyleSheet("font-size: 18px; color: #8F0774; font-weight: bold;");

    connect(&playerGems, &PlayerGems::gemsChanged, this, [=](int newGems){
        gemsLabel->setText("💎 Gems: " + QString::number(newGems));
    });

    // Timer label

    if (difficulty == 0) {
        remainingTime = 20; // 2 minutes
    } else {
        remainingTime = 180; // 3 minutes
    }

    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    QString timeText = QString("Time: %1:%2")
                           .arg(minutes)
                           .arg(seconds, 2, 10, QChar('0'));

    timerLabel = new QLabel(timeText, this);
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setStyleSheet("font-size: 18px; color: #8F0774; font-weight: bold;");

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameController::updateTimer);
    gameTimer->start(1000); // tick every 1 second

    // Free hints label
    freeHintsLabel = new QLabel("Free Hints: " + QString::number(freeHintsCount), this);
    freeHintsLabel->setAlignment(Qt::AlignCenter);
    freeHintsLabel->setFixedWidth(120);
    freeHintsLabel->setStyleSheet("font-size: 18px; color: #8F0774; font-weight: bold;");

    // Wheel label
    wheelResultLabel = new QLabel("", this);
    wheelResultLabel->setAlignment(Qt::AlignCenter);
    wheelResultLabel->setStyleSheet("font-size: 18px; color: #8F0774; font-weight: bold;");

    setUpWheel();

    // Left buttons
    mainMenuButton = new QPushButton("Main Menu", this);
    helpButton = new QPushButton("Help", this);
    mainMenuButton->setFixedSize(80, 30);
    helpButton->setFixedSize(80, 30);

    connect(mainMenuButton, &QPushButton::clicked, this, &GameController::returnToMainMenu);
    connect(helpButton, &QPushButton::clicked, this, &GameController::openHelpScreen);

    QVBoxLayout *leftButtonsLayout = new QVBoxLayout();
    leftButtonsLayout->addStretch();
    leftButtonsLayout->addWidget(mainMenuButton, 0, Qt::AlignCenter);
    leftButtonsLayout->addWidget(helpButton, 0, Qt::AlignCenter);
    leftButtonsLayout->addStretch();
    leftButtonsLayout->setSpacing(10);

    // --- Wheel + guessed letters vertically ---
    QVBoxLayout *wheelLayout = new QVBoxLayout();
    wheelLayout->addWidget(wheel, 0, Qt::AlignCenter);
    wheelLayout->addWidget(wheelResultLabel, 0, Qt::AlignCenter);
    wheelLayout->setSpacing(0);
    wheelLayout->addWidget(guessedLettersBox, 0, Qt::AlignCenter);
    wheelLayout->setAlignment(Qt::AlignCenter);

    // --- Wheel row ---
    QVBoxLayout *labelsLayout = new QVBoxLayout();
    labelsLayout->addWidget(gemsLabel, 0, Qt::AlignCenter);
    labelsLayout->addWidget(freeHintsLabel, 0, Qt::AlignCenter);

    QHBoxLayout *wheelRow = new QHBoxLayout();
    wheelRow->addLayout(leftButtonsLayout);
    wheelRow->addSpacing(0);
    wheelRow->addLayout(wheelLayout);
    wheelRow->addSpacing(0);
    wheelRow->addLayout(labelsLayout);

    // --- Bottom action buttons ---
    spinButton = new QPushButton("Spin Wheel", this);
    buyVowelButton = new QPushButton("Buy Vowel (3 gems)", this);
    buyHintButton = new QPushButton("Buy Hint (5 gems)", this);
    solveButton = new QPushButton("Solve Phrase", this);

    connect(spinButton, &QPushButton::clicked, this, &GameController::spinWheel);
    connect(buyVowelButton, &QPushButton::clicked, this, &GameController::buyVowel);
    connect(buyHintButton, &QPushButton::clicked, this, &GameController::buyHint);
    connect(solveButton, &QPushButton::clicked, this, &GameController::solvePhrase);

    QHBoxLayout *bottomButtons = new QHBoxLayout();
    bottomButtons->addStretch();
    bottomButtons->addWidget(spinButton);
    bottomButtons->addWidget(buyVowelButton);
    bottomButtons->addWidget(buyHintButton);
    bottomButtons->addWidget(solveButton);
    bottomButtons->addStretch();
    bottomButtons->setSpacing(10);

    // --- Main layout ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(categoryLabel);
    mainLayout->addWidget(phraseLabel);
    mainLayout->addLayout(wheelRow);
    mainLayout->addLayout(bottomButtons);
    mainLayout->setSpacing(20);

    setLayout(mainLayout);
}

void GameController::spinWheel()
{
    if (wheel) wheel->spinWheel();
}

void GameController::buyVowel()
{
    if (playerGems.getGems() < 3) {
        QMessageBox *msg = new QMessageBox(this);
        msg->setIcon(QMessageBox::Warning);
        msg->setWindowTitle("Not enough gems");
        msg->setText("Need 3 gems!");

        activeDialogs.append(msg);
        connect(msg, &QDialog::finished, this, [=](){
            activeDialogs.removeOne(msg);
            msg->deleteLater();
        });

        msg->open();
        return;
    }

    playerGems.spendGems(3);

    while (true) {

        QInputDialog *dialog = new QInputDialog(this);
        dialog->setWindowTitle("Buy a Vowel");
        dialog->setLabelText("Enter a vowel (A, E, I, O, U):");
        dialog->setTextValue("");
        dialog->setInputMode(QInputDialog::TextInput);
        dialog->setModal(true);

        // Track in activeDialogs
        activeDialogs.append(dialog);

        // Show the dialog and block until user input
        bool ok = dialog->exec();
        activeDialogs.removeOne(dialog);
        dialog->deleteLater();

        if (!ok) break;

        QString guess = dialog->textValue();

        if (!ok) break;

        if (guess.isEmpty() || guess.length() != 1 || !guess[0].isLetter()) {
            QMessageBox *msg = new QMessageBox(this);
            msg->setIcon(QMessageBox::Warning);
            msg->setWindowTitle("Invalid Input");
            msg->setText("Enter a single vowel.");

            activeDialogs.append(msg);
            connect(msg, &QDialog::finished, this, [=](){
                activeDialogs.removeOne(msg);
                msg->deleteLater();
            });

            msg->open();
            continue;
        }

        QChar letter = guess[0].toUpper();


        if (!(letter == 'A' || letter == 'E' || letter == 'I' || letter == 'O' || letter == 'U')) {
            QMessageBox *msg = new QMessageBox(this);
            msg->setIcon(QMessageBox::Warning);
            msg->setWindowTitle("Invalid Input");
            msg->setText("That's not a vowel.");

            activeDialogs.append(msg);
            connect(msg, &QDialog::finished, this, [=](){
                activeDialogs.removeOne(msg);
                msg->deleteLater();
            });

            msg->open();
            continue;
        }

        if (guessedLetters.contains(letter)) {
            QMessageBox *msg = new QMessageBox(this);
            msg->setIcon(QMessageBox::Information);
            msg->setWindowTitle("Already Guessed");
            msg->setText("This letter was already guessed.");

            activeDialogs.append(msg);

            connect(msg, &QDialog::finished, this, [=](){
                activeDialogs.removeOne(msg);
                msg->deleteLater();

            });

            msg->open();
            continue;
        }

        guessedLetters.insert(letter);

        QString lettersText = "Guessed Letters: ";
        for (auto l : guessedLetters) lettersText += QString(l) + " ";
        guessedLettersBox->setText(lettersText);

        if (phraseHandler->guessLetter(letter)) {
            updateDisplayedPhrase();
            if (phraseHandler->isComplete()) {
                QMessageBox *msg = new QMessageBox(this);
                msg->setIcon(QMessageBox::Information);
                msg->setWindowTitle("You Win!");
                msg->setText("You guessed the full phrase!");

                activeDialogs.append(msg);
                connect(msg, &QDialog::finished, this, [=](){
                    activeDialogs.removeOne(msg);
                    msg->deleteLater();
                });

                msg->open();
            }
        } else {
            QMessageBox *msg = new QMessageBox(this);
            msg->setIcon(QMessageBox::Information);
            msg->setWindowTitle("Incorrect");
            msg->setText("The letter is not in the phrase.");

            activeDialogs.append(msg);
            connect(msg, &QDialog::finished, this, [=](){
                activeDialogs.removeOne(msg);
                msg->deleteLater();
            });

            msg->open();
        }

        break;
    }
}

void GameController::buyHint() {

    if (freeHintsCount > 0) {

        QMessageBox *msg = new QMessageBox(this);
        msg->setIcon(QMessageBox::Question);
        msg->setWindowTitle("Use Free Hint");
        msg->setText("You have a free hint! Use it?");
        msg->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        activeDialogs.append(msg);
        connect(msg, &QDialog::finished, this, [=](int result){
            activeDialogs.removeOne(msg);
            msg->deleteLater();

            if (result == QMessageBox::Yes) {
                freeHintsCount--;
                freeHintsLabel->setText("Free Hints: " + QString::number(freeHintsCount));

                QMessageBox *hint = new QMessageBox(this);
                hint->setIcon(QMessageBox::Information);
                hint->setWindowTitle("Hint");
                hint->setText("Here's your free hint!");

                activeDialogs.append(hint);
                connect(hint, &QDialog::finished, this, [=](){
                    activeDialogs.removeOne(hint);
                    hint->deleteLater();
                });

                hint->open();
            }
        });

        msg->open();
        return;
    }

    if (playerGems.getGems() < 5) {
        QMessageBox *msg = new QMessageBox(this);
        msg->setIcon(QMessageBox::Warning);
        msg->setWindowTitle("Not enough gems");
        msg->setText("Need 5 gems or a free hint!");

        activeDialogs.append(msg);
        connect(msg, &QDialog::finished, this, [=](){
            activeDialogs.removeOne(msg);
            msg->deleteLater();
        });

        msg->open();
        return;
    }

    playerGems.spendGems(5);

    QMessageBox *msg = new QMessageBox(this);
    msg->setIcon(QMessageBox::Information);
    msg->setWindowTitle("Hint");
    msg->setText("Hint purchased with 5 gems!");

    activeDialogs.append(msg);
    connect(msg, &QDialog::finished, this, [=](){
        activeDialogs.removeOne(msg);
        msg->deleteLater();
    });

    msg->open();
}

void GameController::solvePhrase()
{
    if (!phraseHandler || !gameActive) return;  // don't proceed if game ended

    // Stop the game
    gameActive = false;

    if (gameTimer->isActive())
        gameTimer->stop();

    if (wheel)
        wheel->stopSpin();

    closeAllDialogs();          // close any open dialogs
    letterDialogOpen = false;   // reset letter input flag

    bool ok;
    QString playerInput = QInputDialog::getText(this, "Solve Phrase",
                                                "Enter the full phrase:",
                                                QLineEdit::Normal, "", &ok);

    if (!ok || playerInput.isEmpty()) {
        gameActive = true;       // resume game if player cancels
        if (gameTimer) gameTimer->start(1000);
        return;
    }

    QString inputUpper = playerInput.toUpper();
    QString phraseUpper = phraseHandler->getOriginalPhrase().toUpper();

    if (inputUpper == phraseUpper) {
        // Player guessed correctly
        phraseHandler->revealPhrase();
        updateDisplayedPhrase();

        QMessageBox *msg = new QMessageBox(this);
        msg->setWindowTitle("You Won!");
        msg->setText("Congratulations! You guessed the full phrase correctly!");
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg->button(QMessageBox::Ok)->setText("Return to Main Menu");
        msg->button(QMessageBox::Cancel)->setText("Exit Game");

        activeDialogs.append(msg);
        connect(msg, &QDialog::finished, this, [=](int result){
            activeDialogs.removeOne(msg);
            msg->deleteLater();

            if (result == QMessageBox::Ok)
                returnToMainMenu(true);
            else
                qApp->quit();
        });

        msg->open();

    } else {
        // Incorrect guess
        QMessageBox *msg = new QMessageBox(this);
        msg->setWindowTitle("Incorrect");
        msg->setText("Sorry, that was not correct. Keep playing!");

        activeDialogs.append(msg);
        connect(msg, &QDialog::finished, this, [=](){
            activeDialogs.removeOne(msg);
            msg->deleteLater();

            // Resume game
            gameActive = true;
            if (gameTimer) gameTimer->start(1000);
        });

        msg->open();
    }
}

void GameController::closeAllDialogs() {
    for (QDialog* dlg : activeDialogs) {
        if (dlg) {
            dlg->close();
            dlg->deleteLater();
        }
    }
    activeDialogs.clear();
}

void GameController::startNewGame() {

    // 1. Reset guessed letters
    guessedLetters.clear();
    guessedLettersBox->clear();

    // 2. Reset free hints
    freeHintsCount = 0;
    freeHintsLabel->setText("Free Hints: 0");

    // 3. Reset wheel result label
    wheelResultLabel->setText("");

    // 4. Reset phrase
    initializePhrase();      // picks a new random phrase
    updateDisplayedPhrase(); // updates the UI

    // 5. Reset timer based on difficulty
    remainingTime = (difficulty == 0) ? 120 : 180;
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    timerLabel->setText(QString("Time: %1:%2")
                            .arg(minutes)
                            .arg(seconds, 2, 10, QChar('0')));

    // 6. Restart the timer
    if (gameTimer->isActive()) gameTimer->stop();

    gameTimer->start(1000);

    // 7. Reset player gems
    playerGems.resetGems(0);
}


void GameController::returnToMainMenu(bool skipConfirmation) {

    if (gameTimer && gameTimer->isActive())
        gameTimer->stop();

    gameActive = false;

    if (wheel)
        wheel->stopSpin();

    if (!skipConfirmation) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Exit Game",
            "Are you sure you want to exit the game?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            // User cancelled exit, just continue the game
            gameActive = true;
            if (gameTimer && !gameTimer->isActive())
                gameTimer->start(1000);
            return;
        }
    }

    closeAllDialogs();

    MainController *mainMenu = new MainController();
    mainMenu->setWindowTitle("Spin & Solve");
    mainMenu->resize(700, 500);
    mainMenu->show();

    this->close();
}


void GameController::openHelpScreen()
{
    Help *helpScreen = new Help(this);
    helpScreen->setWindowModality(Qt::ApplicationModal);
    helpScreen->show();
}

void GameController::endGame(const QString &title, const QString &message)
{
    // Stop timer
    if (gameTimer->isActive())
        gameTimer->stop();

    // Close all dialogs
    closeAllDialogs();

    // Ask player what to do
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        title,
        message,
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes)
        startNewGame();
    else
        returnToMainMenu(false);
}


void GameController::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap background(":/images/images/background.png");
    painter.drawPixmap(rect(), background);

    QWidget::paintEvent(event);
}
