#pragma once

#include <QThread>
#include "../ThirdParty/GPSd/gps.h"

class GPSWorker: public QThread
{
    Q_OBJECT
private:
    gps_data_t* gpsData;
public:
    GPSWorker(gps_data_t* p_gpsData);
    virtual void run();
signals:
    void onReadyRead();
    void onError();
};
