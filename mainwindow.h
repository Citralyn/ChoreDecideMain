#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "floorplan.h"
#include "chore.h"
#include <QMainWindow>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString username = "";
    QString community = "";
    FloorPlan myFloorPlan;
    QVector<Chore> chores;
    QMap<QString, int> locationToChore;
    QPushButton* currentChore;
    int numberOfExistingPlans = 0;
    int currentLoadButtonX = 0;
    int currentLoadButtonY = 0;
    bool filledIn = false;

protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void goToPage(int pageNumber);
    void getCommunityPreferences(int community);
    void submitLoginInfo();
    void submitSignUpInfo();
    void viewFloorPlan();
    void addChore();
    void bringUpChore(QPushButton *button);
    void completeChore();
    void resetChoreEditor();
    void addRoommate();
    void savePlan();
    void loadPlan(QPushButton *button);
    void addNewPlanDirectory();
    int getNumberOfDirectories();
    void addLoadButton(int i);
    void fillInExisting();
    void clearPreferences();
    void resetExisting();

};
#endif // MAINWINDOW_H
