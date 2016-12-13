#pragma once

#include <QObject>
#include "v2xplatform.h"
#include <QReadWriteLock>

struct PositioningInfo
{
    double Latitude;
    double Longitude;
    double Altitude;
    double Speed;
    double Azimuth;
    double Acceleration;
};

class AccelerationCalculator
{
private:
    double lastSpeed[5];
    PlatformTimestamp lastSpeedTimestamps[5];
public:
    void Reset();
    void EnqueueSpeed(double speed, PlatformTimestamp timestamp);
    double GetAcceleration();
};

class Positioning: public QObject
{
    Q_OBJECT
private:
    PositioningInfo lastPositioningInfo;
    QReadWriteLock rwLock;

    AccelerationCalculator accelerationCalculator;
private slots:
    void onGPSDataRefreshed(PlatformTimestamp timestamp);
public:
    void Initialize();
    void SetPositioningInfo(PositioningInfo newData);
    PositioningInfo GetPositioningInfo();
signals:
    void onPositioningInfoRefreshed(PlatformTimestamp timestamp);
};

extern Positioning* GPositioning;
