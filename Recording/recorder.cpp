#include "recorder.h"
#include <stdexcept>
#include "Sensor/gps.h"
#include "Sensor/can.h"
#include "Sensor/dsrc.h"
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QSettings>
#include <QSemaphore>

Recorder* GRecorder;

//Recorder的事件响应、入队操作目前在主线程上运行
//UDP写入服务器数据库在单独的线程上进行

QQueue<GPSData> GPSDataQueue;
QQueue<PlatformTimestamp> GPSTimestampQueue;
QMutex GPSDataMutex;

QQueue<CANData> CANDataQueue;
QQueue<PlatformTimestamp> CANTimestampQueue;
QMutex CANDataMutex;

QQueue<QByteArray> DSRCDataQueue;
QQueue<PlatformTimestamp> DSRCTimestampQueue;
QMutex DSRCDataMutex;

QSemaphore HasData;

QString RecordDescription;

void Recorder::StartRecording(QString recordDescription)
{
    connect(GGPS, SIGNAL(onDataRefreshed(PlatformTimestamp)), SLOT(onGPSDataRefreshed(PlatformTimestamp)));
    connect(GCAN, SIGNAL(onDataRefreshed(PlatformTimestamp)), SLOT(onCANDataRefreshed(PlatformTimestamp)));
    connect(GDSRC, SIGNAL(onDataRefreshed(PlatformTimestamp, QByteArray)), SLOT(onDSRCDataRefreshed(PlatformTimestamp, QByteArray)));

    RecordDescription = recordDescription;

    recordingThread.start();
}

void Recorder::StopRecording()
{
    recordingThread.terminate();
}

Recorder::Recorder()
{
}

void RecordingThread::run()
{
    socket = new QUdpSocket();
    QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
    auto influxServerIP = QHostAddress(settings.value("Recording/influxServerIP").toString());
    auto influxServerPort = settings.value("Recording/port").toInt();

    qint64 startTime = qint64(QDateTime::currentDateTime().toTime_t()) * 1000000000;

    forever
    {
        {
            HasData.acquire(1);
            bool isEmpty;
            {
                QMutexLocker lock(&GPSDataMutex);
                isEmpty = GPSDataQueue.isEmpty();
            }
            if(!isEmpty)
            {
                GPSData data;
                PlatformTimestamp timestamp;
                {
                    QMutexLocker lock(&GPSDataMutex);
                    data = GPSDataQueue.dequeue();
                    timestamp = GPSTimestampQueue.dequeue();
                }
                QString command = "gps record=\"" + RecordDescription + "\",";
                command += "latitude=";     command += QString::number(data.Latitude, 'g', 15);       command += ",";
                command += "longitude=";    command += QString::number(data.Longitude, 'g', 15);      command += ",";
                command += "speed=";        command += QString::number(data.Speed, 'g', 15);          command += ",";
                command += "azimuth=";      command += QString::number(data.Azimuth, 'g', 15);        command += ",";
                command += "altitude=";     command += QString::number(data.Altitude, 'g', 15);
                command += " ";
                command += QString::number(timestamp.GetTotalNanoseconds() + startTime);

                //qDebug() << command;
                socket->writeDatagram(command.toUtf8(), influxServerIP, influxServerPort);
            } else {
                HasData.release();
            }
        }

        {
            HasData.acquire(1);
            bool isEmpty;
            {
                QMutexLocker lock(&GPSDataMutex);
                isEmpty = CANDataQueue.isEmpty();
            }
            if(!isEmpty)
            {
                CANData data;
                PlatformTimestamp timestamp;
                {
                    QMutexLocker lock(&CANDataMutex);
                    data = CANDataQueue.dequeue();
                    timestamp = CANTimestampQueue.dequeue();
                }
                QString command = "can record=\"" + RecordDescription + "\",";
                command += "brakepedal=";           command += QString::number(data.BrakePedal, 'g', 15);         command += ",";
                command += "accelerationpedal=";    command += QString::number(data.AccelerationPedal, 'g', 15);  command += ",";
                command += "speed=";                command += QString::number(data.Speed, 'g', 15);              command += ",";
                command += "wheelangle=";           command += QString::number(data.WheelAngle, 'g', 15);
                command += " ";
                command += QString::number(timestamp.GetTotalNanoseconds() + startTime);

                //qDebug() << command;

                socket->writeDatagram(command.toUtf8(), influxServerIP, influxServerPort);
            } else {
                HasData.release();
            }
        }

        {
            HasData.acquire(1);
            bool isEmpty;
            {
                QMutexLocker lock(&GPSDataMutex);
                isEmpty = DSRCDataQueue.isEmpty();
            }
            if(!isEmpty)
            {
                QByteArray data;
                PlatformTimestamp timestamp;
                {
                    QMutexLocker lock(&DSRCDataMutex);
                    data = DSRCDataQueue.dequeue();
                    timestamp = DSRCTimestampQueue.dequeue();
                }

                QString command = "dsrc record=\"" + RecordDescription + "\",";
                command += "rawdata=";
                command += "\"";
                command += data.toBase64(QByteArray::Base64UrlEncoding);
                command += "\"";
                command += " ";
                command += QString::number(timestamp.GetTotalNanoseconds() + startTime);

                socket->writeDatagram(command.toUtf8(), influxServerIP, influxServerPort);
            } else {
                HasData.release();
            }
        }

//        timespec tssleep;
//        tssleep.tv_nsec = 0;
//        tssleep.tv_sec = 0;
//        nanosleep(&tssleep,0);
    }
}

void Recorder::onGPSDataRefreshed(PlatformTimestamp timestamp)
{
    auto data = GGPS->GetData();
    {
        QMutexLocker lock(&GPSDataMutex);
        GPSDataQueue.enqueue(data);
        GPSTimestampQueue.enqueue(timestamp);
    }
    HasData.release(1);
}

void Recorder::onCANDataRefreshed(PlatformTimestamp timestamp)
{
    auto data = GCAN->GetData();
    {
        QMutexLocker lock(&CANDataMutex);
        CANDataQueue.enqueue(data);
        CANTimestampQueue.enqueue(timestamp);
    }
    HasData.release(1);
}

void Recorder::onDSRCDataRefreshed(PlatformTimestamp timestamp, QByteArray data)
{
    {
        QMutexLocker lock(&DSRCDataMutex);
        DSRCDataQueue.enqueue(data);
        DSRCTimestampQueue.enqueue(timestamp);
    }
    HasData.release(1);
}
