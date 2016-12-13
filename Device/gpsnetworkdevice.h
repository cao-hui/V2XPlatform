#ifndef GPSNETWORKDEVICE_H
#define GPSNETWORKDEVICE_H

#include "../ThirdParty/GPSd/gps.h"
#include "Sensor/gps.h"
#include <QTimer>
#include <QThread>
#include "abstractdevice.h"
#include "Sensor/abstractsensor.h"

class GPSNetworkDevice: public QObject, public AbstractDevice
{
    Q_OBJECT
private:
    Sensor<GPSData>* sensor;
    gps_data_t gpsData;
public:
    GPSNetworkDevice(Sensor<GPSData>* p_sensor);
    void Initialize();
    void WaitForDevice();
private slots:
    void onReadyRead();
    void onError();
};


#endif // GPSNETWORKDEVICE_H
