#pragma once

#include <QObject>
#include <QReadWriteLock>
#include "Device/abstractdevice.h"
#include "v2xplatform.h"
#include "Recording/player.h"

class AbstractSensor: public QObject
{
    Q_OBJECT
public:
    virtual QString GetName() = 0;    
    AbstractDevice* Device;
signals:
    void onDataRefreshed(PlatformTimestamp);
};

template<typename T>
class Sensor : public AbstractSensor
{
protected:
    T data;
    QReadWriteLock RWLock;
public:
    typedef T DataType;

    virtual void SetData(T newData)
    {
        RWLock.lockForWrite();
        data = newData;
        RWLock.unlock();
        if(GPlayer == NULL)
            emit onDataRefreshed(GV2XPlatform->GetPlatformTime());
        else
            emit onDataRefreshed(GPlayer->GetPlayerTime());
    }

    virtual T GetData()
    {
        RWLock.lockForRead();
        auto ret = data;
        RWLock.unlock();
        return ret;
    }

    Sensor()
    {

    }

    virtual QString GetName();
};

