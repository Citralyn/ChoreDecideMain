#ifndef FLOORPLAN_H
#define FLOORPLAN_H

#include <QWidget>

class FloorPlan : public QWidget
{
    Q_OBJECT
public:
    // Constructors and assignment operators
    FloorPlan(QWidget *parent = nullptr, int community = 0);
    FloorPlan& operator=(const FloorPlan& other);

    // index to community
    int communityIndex = 0;
    QString communityName = "";
    QStringList communities = {"Plaza Verde", "Plaza Verde 2", "VDC", "VDC Norte", "Camino del Sol", "Puerta del Sol"};
    QMap<int, QStringList> communityFloorPlans;

    // floorPlan
    int floorPlanIndex = 0;
    QString floorPlanName;

    // floorPlan image
    QString getImageURL();


private:
    // will use toTitleCase for selection names, and split and join for image names

    QStringList PlazaVerde = {"1 bed 1 bath private", "1 bed 1 bath shared", "2 bed 1 bath", "2 bed 2 bath a shared",
    "2 bed 2 bath b shared", "2 bed 2 bath c shared", "2 bed 2 bath d shared", "2 bed 2 bath g private", "4 bed 2 bath a", "4 bed 4 bath"};
    QStringList PlazaVerde2 = {"1 bed 1 bath b shared",  "2 bed 2 bath e shared", "2 bed 2 bath f shared",
                               "4 bed 2 bath b", "efficiency", "studio"};
    QStringList VDC = {"Not Implemented."};
    QStringList VDCN = {"Not Implemented."};
    QStringList Camino = {"Not Implemented."};
    QStringList Puerta = {"Not Implemented."};
};

#endif // FLOORPLAN_H
