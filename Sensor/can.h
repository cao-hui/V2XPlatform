#pragma once

#include <QReadWriteLock>
#include <QObject>
#include "Sensor/abstractsensor.h"
#include "Device/abstractdevice.h"
#include "v2xplatform.h"
#include "Recording/player.h"

struct CANData
{
    double BrakePedal = 0;
    double AccelerationPedal = 0;
    double Speed = 0;
    double WheelAngle = 0;
};

template<>
inline CANData ConvertDataPointTo(DataPoint dp)
{
    CANData data;
    data.BrakePedal = dp["brakepedal"].toDouble();
    data.AccelerationPedal = dp["accelerationpedal"].toDouble();
    data.WheelAngle = dp["wheelangle"].toDouble();
    data.Speed = dp["speed"].toDouble();

    return data;
}

template<>
inline QString Sensor<CANData>::GetName()
{
    return "can";
}

extern Sensor<CANData>* GCAN;

