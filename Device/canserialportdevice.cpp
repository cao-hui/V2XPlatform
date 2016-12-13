#include "canserialportdevice.h"
#include <QSettings>
#include <QDebug>

bool checksum(QByteArray str){
    int n = 0;
    for(int i = 2;i <= 18;i++)
        n += str[i];
    return (n & 0xFF) == str[19];
}

CANSerialPortDevice::CANSerialPortDevice(Sensor<CANData>* p_sensor)
    :sensor(p_sensor)
{

}

void CANSerialPortDevice::Initialize()
{

    QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
    serialPort->setPortName(settings.value("CAN/portName").toString());
    serialPort->setBaudRate(settings.value("CAN/baudRate").toInt());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    frameDelimiterStart = "\xAA\x55";
    minFrameLength = 20;
}

void CANSerialPortDevice::onData(const QByteArray &data)
{
    const char msgType1Pattern[4] = {(char)0x8A,0x00,0x00,0x00};
    const char msgType2Pattern[4] = {(char)0x8B,0x01,0x00,0x00};
    const char msgType3Pattern[4] = {(char)0xAB,0x00,0x00,0x00};
    const QByteArray msgType1(msgType1Pattern,sizeof(msgType1Pattern)/sizeof(msgType1Pattern[0]));
    const QByteArray msgType2(msgType2Pattern,sizeof(msgType2Pattern)/sizeof(msgType2Pattern[0]));
    const QByteArray msgType3(msgType3Pattern,sizeof(msgType3Pattern)/sizeof(msgType3Pattern[0]));

    CANData newData = GCAN->GetData();
    CANData oldData = newData;

    const QByteArray& msgType = data.mid(5,4);
    const QByteArray& info = data.mid(10,8);

    if(msgType == msgType1)
    {
        int vbreak = info[0]&0xff;
        newData.BrakePedal = ((double)vbreak)*0.019685;
        int vacc = info[1]&0xff;
        newData.AccelerationPedal = ((double)vacc)*0.390625;
    }
    else if(msgType == msgType2)
    {
        int vwheel1 = (info[0]&0xff)*256;
        int vwheel2 = info[1]&0xff;
        double ret = ((double)(vwheel1+vwheel2))*0.1-3277;
        if(qAbs(ret-oldData.WheelAngle)<200 || oldData.WheelAngle == 0)
            newData.WheelAngle = ((double)(vwheel1+vwheel2))*0.1-3277;
    }
    else if(msgType == msgType3)
    {
        int vspeed1 = info[3]&0xff;
        int vspeed2 =(info[2]&0xff)*256;
        newData.Speed = ((double)(vspeed1+vspeed2))*0.01 / 3.6; // Conv km/h to m/s
    }

    if(memcmp(&oldData, &newData, sizeof(CANData)) != 0)
        sensor->SetData(newData);
    //qDebug() << newData.WheelAngle;
}
