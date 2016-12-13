#ifndef CANDEVICE_H
#define CANDEVICE_H

#include "Device/serialportframedeviceex.h"
#include "Sensor/can.h"

class CANSerialPortDevice: public SerialPortFrameDeviceEx
{
private:
    Sensor<CANData>* sensor;
public:
    CANSerialPortDevice(Sensor<CANData>* p_sensor);
    virtual void Initialize() override;
    virtual void onData(const QByteArray& data);
};

#endif // CANDEVICE_H
