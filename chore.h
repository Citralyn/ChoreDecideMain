#ifndef CHORE_H
#define CHORE_H

#include <QWidget>
#include <QDate>

class Chore : public QWidget
{
    Q_OBJECT
public:
    Chore(QWidget *parent = nullptr);
    Chore(const Chore &other);
    Chore& operator=(const Chore& other);

    QString assignedBy;
    QString assignedTo;
    QPoint location;
    QDate dateAssigned;
    QString title;
    QString description;

};

#endif // CHORE_H
