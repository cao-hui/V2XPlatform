#pragma once

#include <QReadWriteLock>
#include <QObject>
#include "Device/abstractdevice.h"
#include "Sensor/abstractsensor.h"
#include "v2xplatform.h"
#include "Recording/player.h"

struct GPSData
{
    int Hour;
    int Minute;
    int Second;
    int Year;
    int Month;
    int Day;
    double Latitude;
    double Longitude;
    double Speed;
    double Azimuth;
    int Quality;
    char Status;
    int SatelliteNumber;
    double Altitude;
};


template<>
inline GPSData ConvertDataPointTo(DataPoint dp)
{
    GPSData data;
    data.Azimuth = dp["azimuth"].toDouble();
    data.Latitude = dp["latitude"].toDouble();
    data.Longitude = dp["longitude"].toDouble();
    data.Speed = dp["speed"].toDouble();
    data.Altitude = dp["altitude"].toDouble();

    return data;
}

template<>
inline QString Sensor<GPSData>::GetName()
{
    return "gps";
}

extern Sensor<GPSData>* GGPS;
