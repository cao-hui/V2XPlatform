#pragma once

#include <QObject>
#include "WorldModel/world.h"

class ActiveSafety: public QObject
{
    Q_OBJECT
public:
    void Initialize();
    double HistoryLong[6];//历史轨迹经度
    double HistoryLati[6];//历史轨迹纬度
    double HistoryHeading[6];//历史轨迹航向角
    bool HistoryBraking[6];//历史轨迹刹车状态

private slots:
    void onVehicleBSMUpdate(Vehicle* vehicle);
    void onSelfVehicleUpdate();
};
