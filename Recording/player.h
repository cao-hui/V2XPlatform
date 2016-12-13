#pragma once

#include <QObject>
#include <QThread>
#include <QMap>
#include <QVariant>
#include "v2xplatform.h"

const int PlayerFPS = 60;
const double RateMultiplier = 1.0;

const int NUM_DATA_POINTS_PER_DOWNLOAD = 100;
const int NUM_MAX_DATA_POINTS_CACHED = 400;

typedef QMap<QString, QVariant> DataPoint;
typedef QList<DataPoint> DataChunk;

template<typename T>
T ConvertDataPointTo(DataPoint dp);

int GetRecordTotalCount(QString measurementName, QString recordName);

DataChunk GetRecordChunk(QString measurementName, QString recordName, int Start, int Length);

template<typename> class SingleSensorPlayer;
template<typename> class Sensor;

class Player: public QThread
{
    Q_OBJECT
private:
    PlatformTimestamp currentPlayerTime;
    QString recordName;
    QList<class AbstractSensor*> registeredSensors;

public:
    Player();
    bool IsGlobalPaused;
    PlatformTimestamp GetPlayerTime();

    void SetRecordName(QString p_name);

    template<typename T>
    void RegisterSensorAndCreateDevice(Sensor<T>* sensor)
    {
        registeredSensors.push_back(sensor);

        sensor->Device = new SingleSensorPlayer<T>(sensor, recordName);
    }

    virtual void run();
};

extern Player* GPlayer;
