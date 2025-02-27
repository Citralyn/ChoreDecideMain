#include "extra.h"

extra::extra(QWidget *parent, Ui::MainWindow *ui)
    : QWidget{parent}, ui(ui)
{
}

void extra::doStuff() {
    qDebug() << "im doing stuff!";
}
