#pragma once

#include "J2735/BasicSafetyMessage.h"
#include <QByteArray>

struct BSMBlob
{
    int carID = -1;
    double latitude = 0;
    double longitude = 0;
    double speed = 0;
    double heading = 0;
    double wheelAngle = 0;
    bool carBrake = 0;
    double acceleration = 0;
};

namespace BSMMessage
{
    QByteArray Encode(BSMBlob& blob);

    BSMBlob Decode(QByteArray data);

    void Free(BasicSafetyMessage* msg);
}

