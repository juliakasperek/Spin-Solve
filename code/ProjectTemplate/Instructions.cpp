#include "Instructions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

Instructions::Instructions(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(700, 500);

    // background image
    QLabel *background = new QLabel(this);
    QPixmap pix(":/images/images/Instructions.png");
    background->setPixmap(pix.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(0, 0, width(), height());
    background->lower(); // send it behind all other widgets

    // back button (top left)
    backButton = new QPushButton("← Back", this);
    backButton->setStyleSheet("background-color: #EBD1E5; color: #5C1F53; font-size: 16px;");
    backButton->setFixedSize(100, 35);
    backButton->move(10, 10);

    connect(backButton, &QPushButton::clicked, this, &Instructions::backToMenu);

    // instructions text
    instructionsLabel = new QLabel(this);
    instructionsLabel->setText(
        "<b>Welcome to Spin & Solve!</b><br><br>"
        "How to Play:<br>"
        "1. Click 'Start Game' and select your difficulty level.<br>"
        "2. To guess a letter, you must first spin the wheel, which helps you earn gems or prizes if you guess the letter correctly. "
        "If your guess is wrong, you won’t earn those bonuses.<br>"
        "3. Guess consonants to reveal letters in the hidden phrase.<br>"
        "4. Use your gems to buy vowels or get hints.<br>"
        "5. If you decide to guess the phrase and you are incorrect, you can still continue playing.<br>"
        "6. Solve the phrase before time runs out to win!<br><br>"
        "Good luck, contestant!"
        );

    instructionsLabel->setWordWrap(true);
    instructionsLabel->setAlignment(Qt::AlignCenter);
    instructionsLabel->setMaximumWidth(390);
    instructionsLabel->setStyleSheet("font-size: 16px; color: #5C1F53;");

    // layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addSpacing(70); // space below back button
    mainLayout->addWidget(instructionsLabel);
    mainLayout->setAlignment(instructionsLabel, Qt::AlignCenter);
    setLayout(mainLayout);
}
