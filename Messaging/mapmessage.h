#ifndef MAPMSG_H
#define MAPMSG_H

#include <QMap>
#include <QList>
#include "MapData.h"
//#include "light.h"

struct Point {
    long latitude;
    long longitude;
};

struct Lane {
    long laneNumber;
    long lanewidth;
};

struct Path {
    Point junction;
    long lanewidth;
    QList<Lane> lanes;
};

struct Intersect {
    int         id;
    Point       center; // Location about center point of this intersection
    //Light       light;  // Information about light in this intersection
    QList<Path> paths;
};


class MapMsg
{
public:
    QMap<int, Intersect> intersects;
    QByteArray EncodeToMapMessage();
    bool DecodeFromMapMessage(QByteArray &data);

private:
    void Printf();
};

#endif // MAPMSG_H
