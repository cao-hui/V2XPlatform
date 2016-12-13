#include "player.h"

#include <QMutex>
#include <QWaitCondition>
#include <QtNetwork>
#include <QMap>
#include <QJsonObject>

#include "Sensor/gps.h"
#include "Sensor/can.h"
#include "Sensor/dsrc.h"

#include "v2xplatform.h"

#include "singlesensorplayer.hpp"

Player* GPlayer;

QJsonDocument GetResponse(QString query)
{
    qDebug() << query;
    QNetworkAccessManager manager;
    QEventLoop loop;

    QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
    auto influxServerIP = "http://" + settings.value("Recording/influxServerIP").toString()
            + ":8086/query?db=" + settings.value("Recording/database").toString() + "&q=" + query;

    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(influxServerIP)));

    loop.connect(reply, SIGNAL(finished()),
                 &loop, SLOT(quit()));

    loop.exec();

    return QJsonDocument::fromJson(reply->readAll());
}

int GetRecordTotalCount(QString measurementName, QString recordName)
{
    auto json = GetResponse("select+count(record)+from+" + measurementName+ "+where+record%3D%27" + recordName + "%27");

    auto descs = json.object()["results"].toArray()[0].toObject()["series"].toArray()[0].toObject()["values"].toArray();

    qDebug() << descs[0].toArray()[1].toInt();
    return descs[0].toArray()[1].toInt();
}

DataChunk GetRecordChunk(QString measurementName, QString recordName, int Start, int Length)
{
    auto json = GetResponse("select+*+from+" + measurementName+ "+where+record%3D%27" + recordName + "%27+limit+" + QString::number(Length)+ "+offset+" + QString::number(Start) + "&epoch=ms");

    QJsonArray columns = json.object()["results"].toArray()[0].toObject()["series"].toArray()[0].toObject()["columns"].toArray();

    auto values = json.object()["results"].toArray()[0].toObject()["series"].toArray()[0].toObject()["values"].toArray();

    typedef QMap<QString, QVariant> DataPoint;
    QList<DataPoint> dataChunk;

    for(auto rawdp: values)
    {
        DataPoint dataPoint;
        QJsonArray dataColumns = rawdp.toArray();
        for(int i=0; i<dataColumns.size(); i++)
        {
            dataPoint[columns[i].toString()] = dataColumns[i].toVariant();
        }

        dataChunk.push_back(dataPoint);
    }

    return dataChunk;
}


Player::Player()
    :IsGlobalPaused(true)
{

}

void Player::SetRecordName(QString p_name)
{
    recordName = p_name;
}

PlatformTimestamp Player::GetPlayerTime()
{
    return currentPlayerTime;
}

void Player::run()
{
    currentPlayerTime = PlatformTimestamp(std::numeric_limits<long long>::max(), std::numeric_limits<long long>::max());
    for(auto it: registeredSensors)
        currentPlayerTime = std::min(currentPlayerTime, static_cast<AbstractSingleSensorPlayer*>(it->Device)->GetStartTime());

    forever
    {
        static PlatformTimestamp oldtp = PlatformTimestamp::GetSystemMonotonicTime();
        PlatformTimestamp tp = PlatformTimestamp::GetSystemMonotonicTime();
        PlatformTimestamp tpdiff = PlatformTimestamp::Diff(oldtp, tp);
        if(tpdiff.Nanoseconds >= 1000000000 / PlayerFPS || tpdiff.Seconds >= 1)
        {
            if(!IsGlobalPaused)
            {
                currentPlayerTime = currentPlayerTime+tpdiff;

                for(auto it: registeredSensors)
                    static_cast<AbstractSingleSensorPlayer*>(it->Device)->Tick();
            }

            oldtp = tp;
        }
//        sched_yield();

        timespec tssleep;
        tssleep.tv_nsec = 0;
        tssleep.tv_sec = 0;
        nanosleep(&tssleep,0);
    }
}
