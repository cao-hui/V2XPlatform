#include <sched.h>
#include <time.h>
#include <tclap/CmdLine.h>
#include <QJsonDocument>
#include "qdebug.h"
#include "v2xplatform.h"
#include "Sensor/gps.h"
#include "Sensor/can.h"
#include "Sensor/dsrc.h"
#include "Fusion/positioning.h"
#include "Recording/recorder.h"
#include "Messaging/bsmmessage.h"
#include "WorldModel/world.h"
#include "Application/ActiveSafety/activesafety.h"
#include "Application/SpeedGuidance/judge.h"
#include "Device/gpsnetworkdevice.h"
#include "Device/gpsserialportdevice.h"
#include "Device/canserialportdevice.h"
#include "Device/dsrcudpdevice.h"
#include "Recording/player.h"
#include "Recording/singlesensorplayer.hpp"
#include "Sensor/abstractsensor.h"
#include "Push/pushhmi.h"
#include "UI/mainwindow.h"

V2XPlatform* GV2XPlatform;

PlatformTimestamp PlatformTimestamp::Diff(PlatformTimestamp start, PlatformTimestamp end)
{
    if ((end.Nanoseconds - start.Nanoseconds) < 0)
        return PlatformTimestamp(end.Seconds - start.Seconds - 1, end.Nanoseconds - start.Nanoseconds + 1000000000);
    else
        return PlatformTimestamp(end.Seconds - start.Seconds    , end.Nanoseconds - start.Nanoseconds);
}

PlatformTimestamp PlatformTimestamp::GetSystemMonotonicTime()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return PlatformTimestamp(ts);
}

PlatformTimestamp PlatformTimestamp::operator+(const PlatformTimestamp& operand)
{
    return PlatformTimestamp(this->Seconds+operand.Seconds+(this->Nanoseconds+operand.Nanoseconds)/1000000000, (this->Nanoseconds+operand.Nanoseconds)%1000000000);
}

bool PlatformTimestamp::operator>(const PlatformTimestamp& operand) const
{
    return Seconds > operand.Seconds || (Seconds == operand.Seconds && Nanoseconds>operand.Nanoseconds);
}

bool PlatformTimestamp::operator<(const PlatformTimestamp& operand) const
{
    return Seconds < operand.Seconds || (Seconds == operand.Seconds && Nanoseconds<operand.Nanoseconds);
}


qint64 PlatformTimestamp::GetTotalNanoseconds()
{
    return qint64(Seconds) * 1000000000 + Nanoseconds;
}

qint64 PlatformTimestamp::GetTotalMilliseconds()
{
    return Seconds * 1000 + Nanoseconds / 1000000;
}


V2XPlatform::V2XPlatform()
{
}

PlatformTimestamp V2XPlatform::GetPlatformTime()
{
    //第一次调用时platformStartTime为0，恰好将platformStarTime初始化为当前时间
    return PlatformTimestamp::Diff(platformStartTime, PlatformTimestamp::GetSystemMonotonicTime());
}

PlatformTimestamp V2XPlatform::GetPlatformStartTime()
{
    return platformStartTime;
}


void V2XPlatform::Initialize(int argc, char *argv[])
{
    QString recordTagName;
    QString replayTagName;

    {
        TCLAP::CmdLine cmd("Tongji University");
        TCLAP::ValueArg<std::string> recordName("r", "record", "If specified, sensor data will be recorded and tagged with the specified name.", false, "", "string");
        TCLAP::ValueArg<std::string> replayName("p", "replay", "If specified, platform will run in replay mode and replay the specified record.", false, "", "string");
        TCLAP::SwitchArg bListTags("l", "list", "Show the list of records");
        cmd.add(recordName);
        cmd.add(replayName);
        cmd.add(bListTags);
        cmd.parse(argc, argv);

        if(bListTags.getValue())
        {
            QNetworkAccessManager manager;
            QEventLoop loop;

            QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
            auto influxServerIP = "http://" + settings.value("Recording/influxServerIP").toString() + ":8086/query?q=show+tag+values+with+key+%3D+%22record%22&db=V2XPlatform";

            QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(influxServerIP)));

            loop.connect(reply, SIGNAL(finished()),
                         &loop, SLOT(quit()));

            loop.exec();

            auto json = QJsonDocument::fromJson(reply->readAll());

            auto descs = json.object()["results"].toArray()[0].toObject()["series"].toArray()[0].toObject()["values"].toArray();

            qDebug() << "List of records:" << descs.size();
            for(auto i: descs)
            {
                qDebug() << i.toArray()[1].toString();
            }

            if(bListTags.getValue())
                return; // Exit platform at here
        }

        recordTagName = recordName.getValue().c_str();
        replayTagName = replayName.getValue().c_str();
    }

    qRegisterMetaType<PlatformTimestamp>("PlatformTimestamp");

    platformStartTime = PlatformTimestamp::GetSystemMonotonicTime();

    GGPS = new Sensor<GPSData>();
    GCAN = new Sensor<CANData>();
    GDSRC = new Sensor<DSRCData>();

    if(replayTagName == "")
    {
        GGPS->Device = new GPSNetworkDevice(GGPS);//new GPSSerialPortDevice(GGPS);
        GCAN->Device = new CANSerialPortDevice(GCAN);
        GDSRC->Device = new DSRCUDPDevice(GDSRC);
    } else {
        GPlayer = new Player();
        GPlayer->SetRecordName(replayTagName);
        GPlayer->RegisterSensorAndCreateDevice(GGPS);
        GPlayer->RegisterSensorAndCreateDevice(GCAN);
        GPlayer->RegisterSensorAndCreateDevice(GDSRC);
    }

    GGPS->Device->Initialize();
    GCAN->Device->Initialize();
    GDSRC->Device->Initialize();

    GPositioning = new Positioning();
    GPositioning->Initialize();

    GGPS->Device->WaitForDevice();
    GCAN->Device->WaitForDevice();
    GDSRC->Device->WaitForDevice();

    GWorld = new World();
    GWorld->Initialize();

    GRecorder = new Recorder();

    if(recordTagName != "")
    {
        qDebug() << "Start recording into" << recordTagName;
        GRecorder->StartRecording(recordTagName);
    }

    static ActiveSafety activeSafetyApp;
    activeSafetyApp.Initialize();

    static Judge_guidance judge_guidance;
    judge_guidance.Initialize();

    if(replayTagName != "")
    {
        GPlayer->start();
        GPlayer->IsGlobalPaused = false;
    }

//    {
//        Intersect inst;
//        inst.id = 1;
//        inst.paths.append(Path());
//        inst.paths.append(Path());
//        inst.paths.append(Path());
//        inst.paths.append(Path());
//        MapMsg msg;
//        msg.intersects[1] = inst;
//    GWorld->UpdateFromMAPMessage(msg);
//    }

    GMainWindow = new mainwindow();


    /******************************************************/

    PushHmi *push=new PushHmi();
    push->init("192.168.1.101",4040);//平板开放端口4040
    //push->startPushing();
    //delete push;

    /******************************************************/
}
