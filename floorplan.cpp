#include "floorplan.h"

FloorPlan::FloorPlan(QWidget *parent, int community)
    : QWidget{parent}
{
    communityIndex = community;
    communityName = communities[community];

    communityFloorPlans.insert(0, PlazaVerde);
    communityFloorPlans.insert(1, PlazaVerde2);
    communityFloorPlans.insert(2, VDC);
    communityFloorPlans.insert(3, VDCN);
    communityFloorPlans.insert(4, Camino);
    communityFloorPlans.insert(5, Puerta);
}

FloorPlan& FloorPlan::operator=(const FloorPlan& other) {
    if (this == &other) {
        return *this;
    }

    communityIndex = other.communityIndex;
    communityName = other.communityName;

    return *this;
}

QString FloorPlan::getImageURL() {
    QString floorplan = communityFloorPlans[communityIndex][floorPlanIndex].split(" ").join("");
    QString url = "";

    switch (communityIndex) {
    case 0:
        url = "url(:/pv_layouts/" + floorplan + ".png)";
        break;
    case 1:
        url = "url(:/pv2_layouts/" + floorplan + ".png)";
        break;
    case 2:
        url = "url(:/vdc_layouts/" + floorplan + ".png)";
        break;
    case 3:
        url = "url(:/vdcn_layouts/" + floorplan + ".png)";
        break;
    case 4:
        url = "url(:/camino_layouts/" + floorplan + ".png)";
        break;
    case 5:
        url = "url(:/puerta_layouts/" + floorplan + ".png)";
        break;
    default:
        url = "url(:/pv_layouts/" + floorplan + ".png)";
        break;
    }

    return url;
}
