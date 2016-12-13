#include "world.h"
#include "Messaging/bsmmessage.h"
#include "Fusion/positioning.h"
#include "Sensor/can.h"
#include "Sensor/dsrc.h"
#include <QDebug>
#include <QSettings>

Vehicle::Vehicle()
{
}

SelfVehicle::SelfVehicle()
{
    QSettings settings("PlatformConfig.ini", QSettings::IniFormat);
    int id = settings.value("Vehicle/ID", 0).toInt();
    if(id == 0) {
        //TODO: specify ID in some waey
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, std::numeric_limits<int>::max());
        id = dis(gen);
        settings.setValue("Vehicle/ID", id);
    }

    this->ID = id;

    qDebug() << "SelfVehicle ID:" << this->ID;

    connect(GPositioning, SIGNAL(onPositioningInfoRefreshed(PlatformTimestamp)), SLOT(onPositioningInfoRefreshed(PlatformTimestamp)));
    connect(GCAN, SIGNAL(onDataRefreshed(PlatformTimestamp)), SLOT(onPositioningInfoRefreshed(PlatformTimestamp)));
}

void SelfVehicle::UpdateFromBSMMessage(BSMBlob bsmmessage)
{
    //override update mechanics in parent class
    return;
}

void SelfVehicle::onPositioningInfoRefreshed(PlatformTimestamp timestamp)
{
    BSMBlob blob;
    auto position = GPositioning->GetPositioningInfo();
    auto candata = GCAN->GetData();
    blob.carID = this->ID;
    blob.latitude = position.Latitude;
    blob.longitude = position.Longitude;
    blob.speed = position.Speed;
    blob.heading = position.Azimuth;
    blob.wheelAngle = candata.WheelAngle;
    blob.acceleration = position.Acceleration;
    blob.carBrake = false;

    this->Latitude = position.Latitude;
    this->Longitude = position.Longitude;
    this->Speed = position.Speed;
    this->Heading = position.Azimuth;
    this->WheelAngle = candata.WheelAngle;
    this->Acceleration = position.Acceleration;
    this->IsBraking = false;

    GDSRC->Broadcast(BSMMessage::Encode(blob));

    emit onSelfPositioningInfoRefreshed();
}

void Vehicle::ResetTTLTimer()
{
    this->TimeToLive = DEFAULT_VEHICLE_TTL;
}

void Vehicle::UpdateFromBSMMessage(BSMBlob bsmmessage)
{
    this->Latitude = bsmmessage.latitude;
    this->Longitude = bsmmessage.longitude;
    this->Speed = bsmmessage.speed;
    this->Heading = bsmmessage.heading;
    this->WheelAngle = bsmmessage.wheelAngle;
    this->Acceleration = bsmmessage.acceleration;

    if(!this->IsBraking && bsmmessage.carBrake)
    {
        //emit onVehicleBrake signal
    }

    this->IsBraking = bsmmessage.carBrake;

    ResetTTLTimer();

//    qDebug() << "Update vehicle status from BSM: " << this->ID << this->Latitude << this->Longitude << this->WheelAngle;

}
