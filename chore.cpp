#include "chore.h"

Chore::Chore(QWidget *parent)
    : QWidget{parent}
{}

Chore::Chore(const Chore &other) {
    assignedBy = other.assignedBy;
    assignedTo = other.assignedTo;
    dateAssigned = other.dateAssigned;
    location = other.location;
    title = other.title;
    description = other.description;
}

Chore& Chore::operator=(const Chore& other) {
    if (this == &other) {
        return *this;
    }

    assignedBy = other.assignedBy;
    assignedTo = other.assignedTo;
    dateAssigned = other.dateAssigned;
    location = other.location;
    title = other.title;
    description = other.description;

    return *this;
}
