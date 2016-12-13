#include "positioning.h"
#include "Sensor/gps.h"
#include "Sensor/can.h"
#include "qmath.h"

Positioning* GPositioning;

void AccelerationCalculator::Reset()
{
    for(int i = 0; i < 5; i++)
    {
        lastSpeed[i] = 0;
        lastSpeedTimestamps[i].Seconds = 10000 * i;
        lastSpeedTimestamps[i].Nanoseconds = 0;
    }
}

void AccelerationCalculator::EnqueueSpeed(double speed, PlatformTimestamp timestamp)
{
    for(int i = 0; i < 4; i++)
    {
        lastSpeed[i] = lastSpeed[i+1];
        lastSpeedTimestamps[i] = lastSpeedTimestamps[i+1];
    }
    lastSpeed[4] = speed;
    lastSpeedTimestamps[4] = timestamp;
}

double AccelerationCalculator::GetAcceleration()
{
    //加速度计算，公式需要更新
    double x0,x1,x2,x3,x4;
    x0=lastSpeedTimestamps[0].Seconds+(lastSpeedTimestamps[0].Nanoseconds)*qPow(10,-9);
    x1=lastSpeedTimestamps[1].Seconds+(lastSpeedTimestamps[1].Nanoseconds)*qPow(10,-9);
    x2=lastSpeedTimestamps[2].Seconds+(lastSpeedTimestamps[2].Nanoseconds)*qPow(10,-9);
    x3=lastSpeedTimestamps[3].Seconds+(lastSpeedTimestamps[3].Nanoseconds)*qPow(10,-9);
    x4=lastSpeedTimestamps[4].Seconds+(lastSpeedTimestamps[4].Nanoseconds)*qPow(10,-9);
    double L0,L1,L2,L3,L4;
    L0=(x0-x1)*(x0-x2)*(x0-x3)*(x0-x4);
    L1=(x1-x0)*(x1-x2)*(x1-x3)*(x1-x4);
    L2=(x2-x0)*(x2-x1)*(x2-x3)*(x2-x4);
    L3=(x3-x0)*(x3-x1)*(x3-x2)*(x3-x4);
    L4=(x4-x0)*(x4-x1)*(x4-x2)*(x4-x3);
    double replace_0,replace_1,replace_2,replace_3,replace_4;
    replace_0=(lastSpeed[0]/L0)*((2*x2-(x1+x2))*(x2*x2-(x3+x4)*x2+x3*x4)+(2*x2-(x3+x4))*(x2*x2-(x1+x2)*x2+x1*x2));
    replace_1=(lastSpeed[1]/L1)*((2*x2-(x0+x2))*(x2*x2-(x3+x4)*x2+x3*x4)+(2*x2-(x3+x4))*(x2*x2-(x0+x2)*x2+x0*x2));
    replace_2=(lastSpeed[2]/L2)*((2*x2-(x0+x1))*(x2*x2-(x3+x4)*x2+x3*x4)+(2*x2-(x3+x4))*(x2*x2-(x0+x1)*x2+x0*x1));
    replace_3=(lastSpeed[3]/L3)*((2*x2-(x1+x2))*(x2*x2-(x0+x4)*x2+x0*x4)+(2*x2-(x0+x4))*(x2*x2-(x1+x2)*x2+x1*x2));
    replace_4=(lastSpeed[4]/L4)*((2*x2-(x1+x2))*(x2*x2-(x3+x0)*x2+x3*x0)+(2*x2-(x3+x0))*(x2*x2-(x1+x2)*x2+x1*x2));
    return (replace_0+replace_1+replace_2+replace_3+replace_4);
    //加速度计算，公式需要更新
    //return (3 * lastSpeed[0] - 16 * lastSpeed[1] + 36 * lastSpeed[2] - 48 * lastSpeed[3] + 25 * lastSpeed[4])/ (12 * 0.2);
}

void Positioning::SetPositioningInfo(PositioningInfo newData)
{
    rwLock.lockForWrite();
    lastPositioningInfo = newData;
    rwLock.unlock();
}

PositioningInfo Positioning::GetPositioningInfo()
{
    rwLock.lockForRead();
    PositioningInfo data = lastPositioningInfo;
    rwLock.unlock();
    return data;
}

void Positioning::Initialize()
{
    connect(GGPS, SIGNAL(onDataRefreshed(PlatformTimestamp)), SLOT(onGPSDataRefreshed(PlatformTimestamp)), Qt::QueuedConnection);

    accelerationCalculator.Reset();
}

void Positioning::onGPSDataRefreshed(PlatformTimestamp timestamp)
{
    GPSData gpsData = GGPS->GetData();

    accelerationCalculator.EnqueueSpeed(gpsData.Speed, timestamp);

    PositioningInfo lastInfo = GetPositioningInfo();

    lastInfo.Acceleration = accelerationCalculator.GetAcceleration();
    lastInfo.Latitude = gpsData.Latitude;
    lastInfo.Longitude = gpsData.Longitude;
    lastInfo.Altitude = gpsData.Altitude;
    if(gpsData.Azimuth != 0 || (gpsData.Azimuth == 0 && gpsData.Speed>1))
        lastInfo.Azimuth = gpsData.Azimuth;
    lastInfo.Speed = gpsData.Speed;

    //CANData data = GCAN->GetData();
    //lastInfo.Speed = data.Speed;
    SetPositioningInfo(lastInfo);

    emit onPositioningInfoRefreshed(timestamp);
}
