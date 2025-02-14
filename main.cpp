#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QIcon icon(":/general/ChoreDeciderIcon.icns");
    a.setWindowIcon(icon);

    MainWindow w;
    w.setWindowIcon(icon);
    w.setWindowTitle("Chore Decider :^)");
    w.setFixedSize(w.geometry().width(),w.geometry().height());
    w.show();

    return a.exec();
}
