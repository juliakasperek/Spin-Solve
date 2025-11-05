#include "MainController.h"
#include "Difficulty.h"
#include "GameController.h"
#include "Game.h"
#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>

MainController::MainController(QWidget *parent) : QWidget(parent) {
    setStyleSheet("MainController {"
                  "background-color: lightpink;"
                  "}");


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(15);               // space between buttons
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);

    startButton = new QPushButton("Start Game", this);
    startButton->setFixedSize(150, 40);
    instructionsButton = new QPushButton("Instructions", this);
    instructionsButton->setFixedSize(150, 40);

    layout->addWidget(startButton);
    layout->addWidget(instructionsButton);

    connect(startButton, &QPushButton::clicked, this, &MainController::startGame);
    connect(instructionsButton, &QPushButton::clicked, this, &MainController::showInstructions);

    setLayout(layout);
}


void MainController::startGame() {
    Difficulty dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        int difficulty = dlg.getSelectedDifficulty();
        qDebug() << "Difficulty selected:" << (difficulty == 0 ? "Easy" : "Hard");

        // Open the game screen
        GameController *gameScreen = new GameController(difficulty);

        // When the game screen closes, show the main menu again
        connect(gameScreen, &QWidget::destroyed, this, [=]() {
            this->show();
        });

        gameScreen->setWindowTitle("Spin & Solve - Game");
        gameScreen->resize(700, 500);
        gameScreen->show();

        // Hide the main menu instead of closing it
        this->hide();
    }
}

void MainController::showInstructions() {
    QMessageBox::information(this, "Instructions", "Instructions would appear here!");
    qDebug() << "Instructions pressed";
}

void MainController::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply = QMessageBox::question (
        this,
        "Exit Game",
        "Do you want to leave the game?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}
