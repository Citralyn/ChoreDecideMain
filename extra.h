#ifndef EXTRA_H
#define EXTRA_H

#include <QWidget>
#include "ui_mainwindow.h"
#include <QDebug>

class extra : public QWidget
{
    Q_OBJECT
public:
    explicit extra(QWidget *parent = nullptr, Ui::MainWindow *ui = nullptr);
    Ui::MainWindow *ui;
    void doStuff();
};

#endif // EXTRA_H
