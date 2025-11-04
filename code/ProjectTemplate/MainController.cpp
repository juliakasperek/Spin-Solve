#include "MainController.h"
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
    QMessageBox::information(this, "Start Game", "Game would start here!");
    qDebug() << "Start Game pressed";
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
