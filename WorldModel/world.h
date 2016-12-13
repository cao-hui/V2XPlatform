#ifndef WORLD_H
#define WORLD_H
#include <QMap>
#include <QList>
#include <QObject>
#include "v2xplatform.h"

#include <QReadWriteLock>
#include "worldobject.h"

class BSMBlob;
class SPATIntersectionState;
class MapMsg;

const double DEFAULT_VEHICLE_TTL = 60;

class Vehicle: public QObject
{
public:
    int ID;
    double Latitude;
    double Longitude;
    double Speed;
    double Heading;
    double WheelAngle;
    double Acceleration;
    bool IsBraking;

    double TimeToLive = DEFAULT_VEHICLE_TTL;

    virtual void UpdateFromBSMMessage(BSMBlob bsmmessage);

    void ResetTTLTimer();

    Vehicle();
};

class SelfVehicle: public Vehicle
{
    Q_OBJECT
public:
    SelfVehicle();
    virtual void UpdateFromBSMMessage(BSMBlob bsmmessage);
signals:
    void onSelfPositioningInfoRefreshed();
private slots:
    void onPositioningInfoRefreshed(PlatformTimestamp timestamp);
};

class World: public QObject
{
    Q_OBJECT
public:
    QMap<int, Vehicle*> Vehicles;
    QMap<int, WorldModel::Intersection*> Intersections;

    SelfVehicle* Self;

    QList<Vehicle*> GetNearbyVehicles(Vehicle *vehicle, double maxDistance = std::numeric_limits<double>::max());//std::numeric_limits<double>::max()返回编译器允许到最大double型数

    void UpdateFromBSMMessage(BSMBlob bsmmessage);
    void UpdateFromSPATMessage(SPATIntersectionState spatmessage);
    void UpdateFromMAPMessage(MapMsg mapmessage);

    void Initialize();

    World();
/****************************************/
    QMap<int, WorldModel::Warning_Data*> Warnings;//int 为与自车碰撞的车辆ID;Warning_Data 的 validCount应初始化为3
    QList<WorldModel::Warning_Data*> GetWarnings();
    void RefreshWarnings();

    QMap<int, WorldModel::Road_Data*> Roads;//int 为红绿灯或路口ID; validCount应初始化为3; 请确保Roads的有效size不大于1，因为hmi最多只能显示一个红绿灯
    QList<WorldModel::Road_Data*> GetRoads();
    void RefreshRoads();
/****************************************/

signals:

    void onVehicleBSMUpdate(Vehicle* vehicle);
};

extern World* GWorld;

#endif // WORLD_H
