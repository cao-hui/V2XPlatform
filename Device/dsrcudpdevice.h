#ifndef DSRCUDPDEVICE_H
#define DSRCUDPDEVICE_H

#include <QObject>
#include <QTimer>
#include <QtNetwork>
#include "Sensor/dsrc.h"
#include "abstractdevice.h"

class DSRCUDPDevice: public QObject, public AbstractDevice
{
    Q_OBJECT
private:
    QUdpSocket socket;
    QHostAddress broadcastIP;
    int remotePort;
    class Sensor<DSRCData>* sensor;

    QTimer testTimer;

private slots:
    void onReadyRead();
    void onTestTimerTick();
public:
    DSRCUDPDevice(Sensor<DSRCData>* p_sensor);
    void Initialize();
    void WaitForDevice();
    void Write(const QByteArray &data);
};

#endif // DSRCUDPDEVICE_H
