#include "dsrcudpdevice.h"
#include <QtCore>
#include <QNetworkInterface>
#include <stdexcept>
#include "Messaging/bsmmessage.h"

void determineNetworkInterface()
{
    QList<QString> possibleMatches;
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    if ( !ifaces.isEmpty() )
    {
        for(int i=0; i < ifaces.size(); i++)
        {
            unsigned int flags = ifaces[i].flags();
            bool isLoopback = (bool)(flags & QNetworkInterface::IsLoopBack);
            bool isP2P = (bool)(flags & QNetworkInterface::IsPointToPoint);
            bool isRunning = (bool)(flags & QNetworkInterface::IsRunning);

            // If this interface isn't running, we don't care about it
            if ( !isRunning ) continue;
            // We only want valid interfaces that aren't loopback/virtual and not point to point
            if ( !ifaces[i].isValid() || isLoopback || isP2P ) continue;
            QList<QHostAddress> addresses = ifaces[i].allAddresses();
            for(int a=0; a < addresses.size(); a++)
            {
              // Ignore local host
              if ( addresses[a] == QHostAddress::LocalHost ) continue;

              // Ignore non-ipv4 addresses
              if ( !addresses[a].toIPv4Address() ) continue;

              QString ip = addresses[a].toString();
              if ( ip.isEmpty() ) continue;
              bool foundMatch = false;
              for (int j=0; j < possibleMatches.size(); j++)
                  if ( ip == possibleMatches[j] ) {
                      foundMatch = true;
                      break;
                  }
              if ( !foundMatch ) {
                  possibleMatches.push_back( ip );
                  qDebug() << "possible address: " << ifaces[i].humanReadableName() << " " << ifaces[i].hardwareAddress() << "->" << ip;
              }
            }
      }
    }
}

DSRCUDPDevice::DSRCUDPDevice(Sensor<DSRCData> *p_sensor)
    :sensor(p_sensor)
{

}

void DSRCUDPDevice::Initialize()
{
    QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
    broadcastIP = QHostAddress(settings.value("DSRCUDP/broadcastIP").toString());
    remotePort = settings.value("DSRCUDP/remotePort").toInt();

    determineNetworkInterface();
    socket.bind(settings.value("DSRCUDP/localPort").toInt());

    connect(&socket, SIGNAL(readyRead()), SLOT(onReadyRead()));

  //  testTimer.setSingleShot(true);
    //testTimer.start(1000);
    connect(&testTimer, SIGNAL(timeout()), SLOT(onTestTimerTick()));
}

void DSRCUDPDevice::WaitForDevice()
{

}

void DSRCUDPDevice::onTestTimerTick()
{
    {
        BSMBlob blob;
        blob.carID = 111;
        blob.latitude = 32.12111;
        blob.longitude = 121.78622;
        blob.speed = 60.123;
        blob.heading = 48.454;
        blob.wheelAngle = -189.000;
        blob.carBrake = true;
        Write(BSMMessage::Encode(blob));
    }
}

void DSRCUDPDevice::Write(const QByteArray &data)
{
    socket.writeDatagram(data, broadcastIP, remotePort);
}

void DSRCUDPDevice::onReadyRead()
{
    while(socket.hasPendingDatagrams())
    {
        auto dataLength = socket.pendingDatagramSize();
        char* buffer = new char[dataLength]();
        socket.readDatagram(buffer, dataLength);
        QByteArray data(buffer, dataLength);
        //qDebug() << data;
        delete buffer;

        sensor->SetData(data);
    }
}
