#include "gpsnetworkdevice.h"
#include <QSettings>
#include <time.h>
#include <QReadWriteLock>
#include <QDebug>
#include <QDateTime>
#include "gpsnetworkhelper.h"
#include "Utils/coordinatetransform.h"
#include <time.h>

QReadWriteLock gpsDataRWLock;

GPSWorker* worker;

GPSNetworkDevice::GPSNetworkDevice(Sensor<GPSData>* p_sensor)
    :sensor(p_sensor)
{

}

void GPSNetworkDevice::Initialize()
{
    worker = new GPSWorker(&gpsData);
    connect(worker, SIGNAL(onError()), SLOT(onError()));


    //Qt::DirectConnection will cause data processed on worker thread
    connect(worker, SIGNAL(onReadyRead()), SLOT(onReadyRead()), Qt::DirectConnection);
}

void GPSNetworkDevice::onReadyRead()
{
    if(std::isnan(gpsData.fix.latitude)) return;

    GPSData newGPSData = sensor->GetData();
    QDateTime datetime;
    datetime.setTime_t(gpsData.fix.time);
    newGPSData.Year = datetime.date().year();
    newGPSData.Month = datetime.date().month();
    newGPSData.Day = datetime.date().day();
    newGPSData.Hour = datetime.time().hour();
    newGPSData.Minute = datetime.time().minute();
    newGPSData.Second = datetime.time().second();
    newGPSData.Latitude = gpsData.fix.latitude;//DegreesMinutesToDecimalDegrees(gpsData.fix.latitude * 100);
    newGPSData.Longitude = gpsData.fix.longitude;//DegreesMinutesToDecimalDegrees(gpsData.fix.longitude * 100);
    newGPSData.Speed = gpsData.fix.speed;
    newGPSData.Quality = gpsData.status;
    newGPSData.SatelliteNumber = gpsData.satellites_used;
    newGPSData.Azimuth = gpsData.fix.track;
    newGPSData.Altitude = gpsData.fix.altitude;

    //qDebug("%f",newGPSData.Latitude);
//    newGPSData.Year = datetime.date().year();
//    newGPSData.Month = datetime.date().month();
//    newGPSData.Day = datetime.date().day();
//    newGPSData.Hour = datetime.time().hour();
//    newGPSData.Minute = datetime.time().minute();
//    newGPSData.Second = datetime.time().second();
//    newGPSData.Latitude = 31.+rand()%100/100000.;//DegreesMinutesToDecimalDegrees(gpsData.fix.latitude * 100);
//    newGPSData.Longitude = 121.+rand()%100/100000.;//DegreesMinutesToDecimalDegrees(gpsData.fix.longitude * 100);
//    newGPSData.Speed = rand()%100;
//    newGPSData.Quality = gpsData.status;
//    newGPSData.SatelliteNumber = gpsData.satellites_used;
//    newGPSData.Azimuth = rand()%100/100.;

    sensor->SetData(newGPSData);
}

void GPSNetworkDevice::WaitForDevice()
{
    {
        QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
        auto IP = settings.value("NetworkGPS/IP").toString();
        auto port = settings.value("NetworkGPS/port").toString();
        qDebug() << "Waiting for network GPS at: " << IP << ":" << port;
    }
    worker->start();
}


void GPSNetworkDevice::onError()
{
    gps_close(&gpsData);

    WaitForDevice();
}

GPSWorker::GPSWorker(gps_data_t *p_gpsData)
    :gpsData(p_gpsData)
{

}

void GPSWorker::run()
{
    QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
    auto IP = settings.value("NetworkGPS/IP").toString();
    auto port = settings.value("NetworkGPS/port").toString();

    //gps_enable_debug(100, stderr);

    forever
    {
        if(gps_open(IP.toLatin1(), port.toLatin1(), gpsData) == 0)
            break;

        int ms = settings.value("DisconnectRecovery/interval", 0).toInt();
        timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
    }

    qDebug() << "Network GPS connected at: " << IP << ":" << port;

    gps_stream(gpsData, WATCH_ENABLE, NULL);

    forever
    {
        timeval Tv;
        Tv.tv_sec = 5;
        Tv.tv_usec = 0;
        fd_set Fds;
        FD_ZERO(&Fds);
        FD_SET(gpsData->gps_fd, &Fds);
        int filestatus = select(gpsData->gps_fd + 1, &Fds, NULL, NULL, &Tv);

        if(filestatus < 0)
        {
            emit onError();
            return;
        } else if(filestatus > 0){
            gpsDataRWLock.lockForWrite();
            int ret = gps_read(gpsData);
            gpsDataRWLock.unlock();
            if(ret)
            {
                //Data format error
                emit onReadyRead();
            } else {
                emit onError();
                return;
            }
        } else {
            //Timeout
            emit onError();
            return;
        }
    }
}

