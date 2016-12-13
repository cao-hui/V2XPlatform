#pragma once

#include "Device/serialportframedeviceex.h"
#include "Sensor/gps.h"

class GPSSerialPortDevice : public SerialPortFrameDeviceEx
{
    Q_OBJECT
private:
    class Sensor<GPSData>* sensor;
public:
    virtual void Initialize() override;
    void onData(const QByteArray& data);
    GPSSerialPortDevice(Sensor<GPSData>*);
};
