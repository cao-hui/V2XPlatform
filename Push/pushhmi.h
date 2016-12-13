#ifndef PUSHHMI_H
#define PUSHHMI_H

#include <QObject>
#include <QTimer>
#include <QtNetwork/QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include "WorldModel/world.h"
#include "Application/ActiveSafety/computetools.h"
#include "Application/ActiveSafety/carData.h"

#include "stdio.h"
#include "stdlib.h"

/**
 * @brief The PushHmi class 用法

    PushHmi *push=new PushHmi();
    push->init("192.168.0.4",4040);//平板开放端口4040
    push->startPushing();
    ...
    delete push;
 */
class PushHmi : public QObject
{
    Q_OBJECT
public:
    explicit PushHmi(QObject *parent = 0);
    ~PushHmi();
    void init(QString ipAddr,int port);
    void startPushing();
signals:

private:
    QString ipAddr;
    int port;
    QTimer *timer;
    FILE* ra;

    QJsonArray updateVehicle_Data();
    QJsonArray updateRoad_Data();
    QJsonArray updateEvent_Data();
    QJsonArray updateWarning_Data();

    QString convertQJsontoQString(QJsonObject JsonObject);
public slots:
    void sendUdp_show();

};

#endif // PUSHHMI_H
