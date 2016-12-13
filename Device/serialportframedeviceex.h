#ifndef SERIALPORTFRAMEDEVICEEX_H
#define SERIALPORTFRAMEDEVICEEX_H

#include "abstractdevice.h"
#include <QSerialPort>
#include <QSemaphore>
#include <QThread>


class SPFDWorkerEx: public QThread
{
    Q_OBJECT
private:
    class SerialPortFrameDeviceEx* device;
    QByteArray buffer;
    void parseNewData(const QByteArray& newData);
private slots:
    void onReadyRead();
    void onError(QSerialPort::SerialPortError error);
public:
    SPFDWorkerEx(SerialPortFrameDeviceEx* p_device);
    void run();

    friend class SerialPortFrameDeviceEx;
};

class SerialPortFrameDeviceEx: public QObject, public AbstractDevice
{
    Q_OBJECT
private:
    SPFDWorkerEx worker;
    QSemaphore workerCreateSem;
    QSemaphore workerWaitForDeviceSem;
protected:
    QSerialPort* serialPort;
    QSerialPort::OpenModeFlag openMode;
    QString portName;
    QByteArray frameDelimiterStart;
    QByteArray frameDelimiterEnd;
    int minFrameLength;
public:
    SerialPortFrameDeviceEx();
    ~SerialPortFrameDeviceEx();

    //WaitForDevice() is called after
    virtual void WaitForDevice();


    virtual void onData(const QByteArray& data) = 0;

    friend class SPFDWorkerEx;
};

#endif // SERIALPORTFRAMEDEVICEEX_H
