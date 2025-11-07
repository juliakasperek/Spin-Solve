#include "GameController.h"
#include "MainController.h"
#include "PhraseLibrary.h"
#include "PlayerGems.h"
#include "PhraseHandler.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QPainter>
#include <QInputDialog>

GameController::GameController(int diff, QWidget *parent)
    : QWidget(parent), difficulty(diff), playerGems(0, this), phraseHandler(nullptr)
{
    initializePhrase();
    setUpUI();
}

void GameController::initializePhrase()
{
    PhraseLibrary library;
    std::string phraseStr;

    if (difficulty == 0) {
        phraseStr = library.getRandomPhrase("easy");
    } else {
        phraseStr = library.getRandomPhrase("hard");
    }

    QString phrase = QString::fromStdString(phraseStr);

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

void GameController::setUpWheel()
{
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
            QMessageBox::information(this, "Wheel Spin", "Wheel landed on: " + landedSegment);

            if (landedSegment.contains("gem")) {
                int gemAmount = landedSegment.split(" ")[0].toInt();
                playerGems.addGems(gemAmount);
            }
            else if (landedSegment == "Free Hint") {
                freeHintsCount++;
                freeHintsLabel->setText("Free Hints: " + QString::number(freeHintsCount));
            }


            // Prompt user to guess a letter
            while (true) {
                bool ok;
                QString guess = QInputDialog::getText(this, "Guess a Letter",
                                                      "Enter a letter (non-vowel only):",
                                                      QLineEdit::Normal, "", &ok);
                if (!ok) break;

                if (guess.isEmpty() || guess.length() != 1) {
                    QMessageBox::warning(this, "Invalid Input", "Enter a single letter.");
                    continue;
                }

                QChar letter = guess[0].toUpper();
                if (letter == 'A' || letter == 'E' || letter == 'I' || letter == 'O' || letter == 'U') {
                    QMessageBox::warning(this, "Invalid Guess", "Please guess a non-vowel!");
                    continue;
                }

                if (guessedLetters.contains(letter)) {
                    QMessageBox::warning(this, "Already Guessed", "This letter was already guessed.");
                    continue;
                }

                guessedLetters.insert(letter);

                // Update guessed letters box
                QString lettersText = "Guessed Letters: ";
                for (auto it = guessedLetters.begin(); it != guessedLetters.end(); ++it)
                    lettersText += QString(*it) + " ";
                guessedLettersBox->setText(lettersText);

                // Update phrase using PhraseHandler
                if (phraseHandler->guessLetter(letter)) {
                    updateDisplayedPhrase();
                    if (phraseHandler->isComplete()) {
                        QMessageBox::information(this, "You Win!", "You guessed the full phrase!");
                    }
                } else {
                    QMessageBox::information(this, "Incorrect", "The letter is not in the phrase.");
                }
                break;
            }
        });
    }
}

void GameController::setUpUI()
{
    // Phrase label
    phraseLabel = new QLabel(displayedPhrase, this);
    phraseLabel->setAlignment(Qt::AlignCenter);
    phraseLabel->setWordWrap(true);
    phraseLabel->setStyleSheet("font-family: monospace; font-size: 30px; font-weight: bold;");

    // Guessed letters box
    guessedLettersBox = new QLineEdit(this);
    guessedLettersBox->setReadOnly(true);
    guessedLettersBox->setAlignment(Qt::AlignCenter);
    guessedLettersBox->setStyleSheet("font-size: 18px;");
    guessedLettersBox->setFixedWidth(400);

    // Gems label
    gemsLabel = new QLabel("💎 Gems: " + QString::number(playerGems.getGems()), this);
    gemsLabel->setAlignment(Qt::AlignCenter);
    gemsLabel->setFixedWidth(100);
    gemsLabel->setStyleSheet("font-size: 18px; color: #8F0774; font-weight: bold;");

    // Free hints label
    freeHintsLabel = new QLabel("Free Hints: " + QString::number(freeHintsCount), this);
    freeHintsLabel->setAlignment(Qt::AlignCenter);
    freeHintsLabel->setFixedWidth(120);
    freeHintsLabel->setStyleSheet("font-size: 18px; color: #8F0774; font-weight: bold;");


    connect(&playerGems, &PlayerGems::gemsChanged, this, [=](int newGems){
        gemsLabel->setText("💎 Gems: " + QString::number(newGems));
    });

    setUpWheel();

    mainMenuButton = new QPushButton("Main Menu", this);
    helpButton = new QPushButton("Help", this);
    mainMenuButton->setFixedSize(80, 30);
    helpButton->setFixedSize(80, 30);

    QVBoxLayout *leftButtonsLayout = new QVBoxLayout();
    leftButtonsLayout->addStretch();
    leftButtonsLayout->addWidget(mainMenuButton, 0, Qt::AlignCenter);
    leftButtonsLayout->addWidget(helpButton, 0, Qt::AlignCenter);
    leftButtonsLayout->addStretch();
    leftButtonsLayout->setSpacing(10);

    QVBoxLayout *wheelLayout = new QVBoxLayout();
    wheelLayout->addWidget(wheel, 0, Qt::AlignCenter);
    wheelLayout->addWidget(guessedLettersBox, 0, Qt::AlignCenter);
    wheelLayout->setAlignment(Qt::AlignCenter);
    wheelLayout->setSpacing(10);

    QHBoxLayout *wheelRow = new QHBoxLayout();
    wheelRow->addLayout(leftButtonsLayout);
    wheelRow->addSpacing(10);
    wheelRow->addLayout(wheelLayout);
    wheelRow->addSpacing(10);

    QVBoxLayout *scoreLayout = new QVBoxLayout();
    scoreLayout->addWidget(freeHintsLabel, 0, Qt::AlignCenter);
    scoreLayout->addWidget(gemsLabel, 0, Qt::AlignCenter);

    QHBoxLayout *scoreWrapper = new QHBoxLayout();
    scoreWrapper->addStretch();
    scoreWrapper->addLayout(scoreLayout);
    scoreWrapper->addSpacing(30);

    wheelRow->addLayout(scoreWrapper);
    wheelRow->addLayout(scoreLayout);

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

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
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
        QMessageBox::warning(this, "Not enough gems", "Need 3 gems!");
        return;
    }
    playerGems.spendGems(3);

    while (true) {
        bool ok;
        QString guess = QInputDialog::getText(this, "Buy a Vowel",
                                              "Enter a vowel (A, E, I, O, U):",
                                              QLineEdit::Normal, "", &ok);
        if (!ok) break;

        if (guess.isEmpty() || guess.length() != 1) {
            QMessageBox::warning(this, "Invalid Input", "Enter a single vowel.");
            continue;
        }

        QChar letter = guess[0].toUpper();
        if (!(letter == 'A' || letter == 'E' || letter == 'I' || letter == 'O' || letter == 'U')) {
            QMessageBox::warning(this, "Invalid Input", "That's not a vowel.");
            continue;
        }

        if (guessedLetters.contains(letter)) {
            QMessageBox::warning(this, "Already Guessed", "This letter was already guessed.");
            continue;
        }

        guessedLetters.insert(letter);

        QString lettersText = "Guessed Letters: ";
        for (auto l : guessedLetters) lettersText += QString(l) + " ";
        guessedLettersBox->setText(lettersText);

        if (phraseHandler->guessLetter(letter)) {
            updateDisplayedPhrase();
            if (phraseHandler->isComplete()) {
                QMessageBox::information(this, "You Win!", "You guessed the full phrase!");
            }
        } else {
            QMessageBox::information(this, "Incorrect", "The letter is not in the phrase.");
        }

        break;
    }
}

void GameController::buyHint()
{
    // first check if the player has a free hint available
    if (freeHintsCount > 0) {
        QMessageBox::StandardButton useHint = QMessageBox::question(
            this,
            "Use Free Hint",
            "You have a free hint! Would you like to use it?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (useHint == QMessageBox::Yes) {
            freeHintsCount--;
            freeHintsLabel->setText("Free Hints: " + QString::number(freeHintsCount));
            QMessageBox::information(this, "Hint", "Here’s your free hint!");
            // add hint logic here later
            return;
        }
    }

    // next, check if the player has enough gems
    if (playerGems.getGems() < 5) {
        QMessageBox::warning(this, "Not enough gems", "Need 5 gems or a free hint!");
        return;
    }

    // spend gems if no free hints used
    playerGems.spendGems(5);
    QMessageBox::information(this, "Hint", "Hint purchased with 5 gems!");
    // add your real hint logic here
}


void GameController::solvePhrase()
{
    if (!phraseHandler) return;

    bool ok;
    QString playerInput = QInputDialog::getText(this, "Solve Phrase",
                                                "Enter the full phrase:",
                                                QLineEdit::Normal, "", &ok);
    if (!ok || playerInput.isEmpty()) return;

    QString inputUpper = playerInput.toUpper();
    QString phraseUpper = phraseHandler->getOriginalPhrase().toUpper();

    if (inputUpper == phraseUpper) {
        phraseHandler->revealPhrase();
        updateDisplayedPhrase();

        QMessageBox msgBox;
        msgBox.setWindowTitle("You Won!");
        msgBox.setText("Congratulations! You guessed the full phrase correctly!");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.button(QMessageBox::Ok)->setText("Return to Main Menu");
        msgBox.button(QMessageBox::Cancel)->setText("Exit Game");

        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok) {
            bypassCloseConfirm = true; // << set the flag
            this->close();             // close this window safely
        }
        else {
            qApp->quit();
        }

    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Incorrect");
        msgBox.setText("Sorry, that was not correct. Keep playing!");
        msgBox.exec();
    }
}


void GameController::closeEvent(QCloseEvent *event)
{
    if (bypassCloseConfirm) {
        // open main menu without asking
        MainController *mainMenu = new MainController();
        mainMenu->setWindowTitle("Spin & Solve");
        mainMenu->resize(700, 500);
        mainMenu->show();

        event->accept(); // close this window
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Exit Game", "Are you sure you want to exit?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (wheel) { delete wheel; wheel = nullptr; }
        if (phraseHandler) { delete phraseHandler; phraseHandler = nullptr; }
        guessedLetters.clear();
        displayedPhrase.clear();
        playerGems.resetGems(10);

        MainController *mainMenu = new MainController();
        mainMenu->setWindowTitle("Spin & Solve");
        mainMenu->resize(700, 500);
        mainMenu->show();

        event->accept();
    } else {
        event->ignore();
    }
}


void GameController::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap background(":/images/images/background.png");
    painter.drawPixmap(rect(), background);

    QWidget::paintEvent(event);
}
