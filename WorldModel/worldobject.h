#pragma once
#include <QList>
#include <QString>

namespace WorldModel
{

struct GeoPosition
{
    double Latitude;
    double Longitude;
};

enum SignalLightColor
{
    Green   = 3,
    Yellow  = 2,
    Red     = 1,
    Error   = 4
};

struct SignalLightState
{
    int rightTimeToChange;
    int straightTimeToChange;
    int leftTimeToChange;
    SignalLightColor rightColor = Error;
    SignalLightColor straightColor = Error;
    SignalLightColor leftColor = Error;
    int rightTime[3];//green yellow red
    int straightTime[3];
    int leftTime[3];
};

struct Lane
{
    int id;
    double width;
    SignalLightState signalLightState;
};

struct Approach {
    int id;
    GeoPosition junction;
    double refLaneWidth;
    QList<Lane> lanes;
};

struct Intersection
{
    int         id;
    GeoPosition center;
    QList<Approach> approaches;
};
//@LiJ
struct Warning_Data{
    QString WarningID;
    int WarningLevel;
    int WarningStr;
    int validCount;
};
struct Road_Data
{
    int SignalPhase;
    int Time_left;
    int Advise_speed;
    int Curren_speed;
    int Mark;
    int Distance;
    int validCount;
};
}
