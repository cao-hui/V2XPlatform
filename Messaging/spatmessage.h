#pragma once

#include "J2735/SPAT.h"
#include <QByteArray>
#include <QList>

enum SPATLightState
{
    green=3,
    yellow=2,
    red=1,
    erro=4
};

struct SPATMovementState
{
    int laneSet;
    int rightTimeToChange;
    int straightTimeToChange;
    int leftTimeToChange;
    SPATLightState rightState = erro;
    SPATLightState straightState = erro;
    SPATLightState leftState = erro;
    int rightTime[3];//green yellow red
    int straightTime[3];
    int leftTime[3];
};

struct SPATIntersectionState
{
    int id = -1;
    int laneCount = 0;
    QList<SPATMovementState> states;
};

namespace SPATMessage
{
    SPATIntersectionState Decode(QByteArray SPATData);
}

extern SPATMovementState TFLightStatus;
