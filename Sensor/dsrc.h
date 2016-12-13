#ifndef DSRC_H
#define DSRC_H

#include <QObject>
#include <stdexcept>
#include <QMap>
#include "v2xplatform.h"
#include "QDebug"

#include "Device/abstractdevice.h"
#include "Sensor/abstractsensor.h"

#include "WorldModel/world.h"
#include "Messaging/bsmmessage.h"
#include "Messaging/spatmessage.h"
#include "Messaging/mapmessage.h"
#include "Recording/player.h"
//class DSRC : public QObject
//{
//    Q_OBJECT
//public:
//    AbstractDevice* Device;
//    void Broadcast(QByteArray data, char channelID = 0);
//    void SetData(QByteArray data);

//signals:
//    void onRawData(PlatformTimestamp timestamp, QByteArray data);
//};


typedef QByteArray DSRCData;

template<>
inline DSRCData ConvertDataPointTo(DataPoint dp)
{
    QByteArray data;
    return data.fromBase64(dp["rawdata"].toString().toUtf8(), QByteArray::Base64UrlEncoding);
}

class DSRCSensorSignalSlot: public AbstractSensor
{
    Q_OBJECT
signals:
    void onDataRefreshed(PlatformTimestamp, QByteArray);
};

template<>
class Sensor<DSRCData>: public DSRCSensorSignalSlot
{
public:
    void SetData(DSRCData data)
    {
        //qDebug()<<(int)data[4];
        //qDebug()<<(int)data[5];
        emit onDataRefreshed(GV2XPlatform->GetPlatformTime(), data);
        switch(data[5]) // The fifth byte is DSRCmsgID
        {
        case 2: // BSM Message
            GWorld->UpdateFromBSMMessage(BSMMessage::Decode(data));
            break;
        case 13: // SPAT Message
            GWorld->UpdateFromSPATMessage(SPATMessage::Decode(data));
            break;
        case 7:
        {
            MapMsg msg;
            msg.DecodeFromMapMessage(data);
            GWorld->UpdateFromMAPMessage(msg);
        }
            break;
        }

        switch(data[4]) // The fifth byte is DSRCmsgID
        {
        case 2: // BSM Message
            GWorld->UpdateFromBSMMessage(BSMMessage::Decode(data));
            break;
        case 13: // SPAT Message
            GWorld->UpdateFromSPATMessage(SPATMessage::Decode(data));
            break;
        case 7:
        {
            MapMsg msg;
            msg.DecodeFromMapMessage(data);
            GWorld->UpdateFromMAPMessage(msg);
        }
            break;
        }
    }

    void Broadcast(DSRCData data)
    {
        Device->Write(data);
    }

    QString GetName()
    {
        return "dsrc";
    }
};

extern Sensor<DSRCData>* GDSRC;

#endif // DSRC_H
