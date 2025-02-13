#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TestFunction(14);
}

void MainWindow::TestFunction(int i) {
    qDebug() << i << Qt::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}
