#include "GameController.h"
#include "MainController.h"
#include "PhraseLibrary.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QRandomGenerator>

GameController::GameController(int diff, QWidget *parent)
    : QWidget(parent), difficulty(diff), gems(10)
{
    // phrase
    PhraseLibrary library;
    std::string phraseStr;
    if (difficulty == 0)
        phraseStr = library.getRandomPhrase("easy");
    else
        phraseStr = library.getRandomPhrase("hard");
    phrase = QString::fromStdString(phraseStr);

    displayedPhrase = "";
    updateDisplayedPhrase();

    // phrase label
    phraseLabel = new QLabel(displayedPhrase, this);
    phraseLabel->setAlignment(Qt::AlignCenter);
    phraseLabel->setWordWrap(true);
    phraseLabel->setStyleSheet("font-size: 30px; font-weight: bold;");

    // guessed letters
    guessedLettersBox = new QLineEdit(this);
    guessedLettersBox->setReadOnly(true);
    guessedLettersBox->setAlignment(Qt::AlignCenter);
    guessedLettersBox->setStyleSheet("font-size: 18px;");

    // buttons
    spinButton = new QPushButton("Spin Wheel", this);
    buyVowelButton = new QPushButton("Buy Vowel (3 gems)", this);
    buyHintButton = new QPushButton("Buy Hint (5 gems)", this);

    connect(spinButton, &QPushButton::clicked, this, &GameController::spinWheel);
    connect(buyVowelButton, &QPushButton::clicked, this, &GameController::buyVowel);
    connect(buyHintButton, &QPushButton::clicked, this, &GameController::buyHint);

    // layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(phraseLabel);
    mainLayout->addWidget(guessedLettersBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(spinButton);
    buttonLayout->addWidget(buyVowelButton);
    buttonLayout->addWidget(buyHintButton);

    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

// display phrase
void GameController::updateDisplayedPhrase()
{
    displayedPhrase = "";
    for (QChar c : phrase) {
        if (c == ' ')
            displayedPhrase += "   ";
        else
            displayedPhrase += "_  ";
    }
    QString html = "<div style='line-height: 1.8;'>" + displayedPhrase + "</div>"; //add line spacing between wrapped rows
    if (phraseLabel)
        phraseLabel->setText(html);
}

// spin wheel
void GameController::spinWheel()
{
    QStringList segments = { "-5 seconds", "-5 seconds", "5 gems", "2 gems", "2 gems",
                            "2 gems", "2 gems", "Free Hint" };

    int index = QRandomGenerator::global()->bounded(segments.size());
    currentSegment = index;
    QString landedSegment = segments[index];

    QMessageBox::information(this, "Wheel Spin", "Wheel landed on: " + landedSegment);

    // guess letter
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
            QMessageBox::warning(this, "Already Guessed", "This letter was already guessed. Please try again!");
            continue;
        }

        guessedLetters.insert(letter);

        // update guessed letters box
        QString lettersText = "Guessed Letters: ";
        for (auto l : guessedLetters)
            lettersText += QString(l) + " ";
        guessedLettersBox->setText(lettersText);

        // TODO: reveal letter in phrase
        break;
    }
}

// buy vowel
void GameController::buyVowel()
{
    if (gems < 3) {
        QMessageBox::warning(this, "Not enough gems", "Need 3 gems!");
        return;
    }
    gems -= 3;
    QMessageBox::information(this, "Buy Vowel", "Vowel purchased (logic not done).");
}

// buy hint
void GameController::buyHint()
{
    if (gems < 5) {
        QMessageBox::warning(this, "Not enough gems", "Need 5 gems!");
        return;
    }
    gems -= 5;
    QMessageBox::information(this, "Hint", "Hint purchased (logic not done).");
}

// close game
void GameController::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Exit Game", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        MainController *mainMenu = new MainController();
        mainMenu->setWindowTitle("Spin & Solve");
        mainMenu->resize(700, 500);
        mainMenu->show();
        event->accept();
    } else {
        event->ignore();
    }
}
