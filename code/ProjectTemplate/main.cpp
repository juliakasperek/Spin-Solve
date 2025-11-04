#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include "MainController.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainController window;
    window.setWindowTitle("Spin & Solve");
    window.resize(700, 500);
    window.setFixedSize(700, 500);
    window.show();

    return app.exec();
}
