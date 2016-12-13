#include "Sensor/gps.h"
#include "gpsserialportdevice.h"
#include "Utils/coordinatetransform.h"
#include <QSettings>
#include <QDebug>

GPSSerialPortDevice::GPSSerialPortDevice(Sensor<GPSData> *p_sensor)
    :sensor(p_sensor)
{
}

void GPSSerialPortDevice::Initialize()
{
    QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
    serialPort->setPortName(settings.value("GPS/portName").toString());
    serialPort->setBaudRate(settings.value("GPS/baudRate").toInt());
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    frameDelimiterStart = "$";
    minFrameLength = 6;
}

void GPSSerialPortDevice::onData(const QByteArray &data)
{
    if(!data.contains('$'))
        return;
    auto tokens = data.split(',');
    if(tokens.size() < 12)
        return;
    if(tokens[0] == "$GPGGA")
    {
        //qDebug() << tokens[6];
        GPSData newData = sensor->GetData();
        newData.Hour = tokens[1].mid(0,2).toInt();
        newData.Minute = tokens[1].mid(2,2).toInt();
        newData.Second = tokens[1].mid(4,2).toInt();
        newData.Quality = tokens[6].toInt();

        newData.Latitude = DegreesMinutesToDecimalDegrees(tokens[2].toDouble());
        if(tokens[3] == "S")
            newData.Latitude = -newData.Latitude;

        newData.Longitude = DegreesMinutesToDecimalDegrees(tokens[4].toDouble());
        if(tokens[5] == "W")
            newData.Longitude = -newData.Longitude;

        newData.SatelliteNumber = tokens[7].toInt();
        newData.Altitude = tokens[11].toDouble();

        sensor->SetData(newData);
    }
    else if(tokens[0] == "$GPRMC")
    {
        GPSData newData = sensor->GetData();
        newData.Year = tokens[1].mid(4,2).toInt();
        newData.Month = tokens[1].mid(2,2).toInt();
        newData.Day = tokens[1].mid(0,2).toInt();
        newData.Hour = tokens[1].mid(0,2).toInt();
        newData.Minute = tokens[1].mid(2,2).toInt();
        newData.Second = tokens[1].mid(4,2).toInt();
        newData.Status = tokens[2][0];

        newData.Latitude = DegreesMinutesToDecimalDegrees(tokens[3].toDouble());
        if(tokens[4] == "S")
            newData.Latitude = -newData.Latitude;

        newData.Longitude = DegreesMinutesToDecimalDegrees(tokens[5].toDouble());
        if(tokens[6] == "W")
            newData.Longitude = -newData.Longitude;

        newData.Speed = tokens[7].toDouble() * 1.852 / 3.6; // Convert knots to m/s

        newData.Azimuth = /*90 -*/ tokens[8].toDouble();
        for(; newData.Azimuth < 0;    newData.Azimuth += 360);
        for(; newData.Azimuth >= 360; newData.Azimuth -= 360);

        sensor->SetData(newData);
    }
}
