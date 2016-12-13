#ifndef CARDATA_H
#define CARDATA_H

#include <QString>
#include <QList>
#include <QPoint>
#include <QVector>
#include "qmath.h"

class carData
{
public:
    carData();
    carData(QString p_VID, bool p_selfvehicle, double p_vLongitude,
            double p_vLatitude, double p_speedmps, double p_heading_gps, double p_wheelangle);

    carData(QString p_VID, bool p_selfvehicle, double p_vLongitude,
            double p_vLatitude, double p_speedmps, double p_heading_gps, double p_wheelangle,bool p_IsBraking);
            //bool p_AtCross,bool p_LeftTurnLight);

    //车辆基础数据
    QString VID;         //车辆ID
    bool ifSelfVehicle;  //是否为自车

    //这三个数据目前没有
    bool BrakeStatus; //刹车状态
    bool AtCross; //是否在路口
    bool LeftTurnLight;//左转灯

    double VLongitude;   //经度
    double VLatitude;    //纬度
    double Speedmps;      //速度，m/s
    double heading_GPS;   //航向角，GPS坐标系，0-360度
    double wheelAngle;    //方向盘转角,单位：°

    double vehicleLength; //车长，单位：m
    double vehicleWidth;  //车宽，单位：m
    double vehicleWheelDis; //
    double heading_Axis;   //航向角，直角坐标系

    QPointF startPoint;  //车辆起始点（直角坐标系）
    QVector<QPointF> carLine;   //车辆轨迹点（直角坐标系）
    QList<double> rotateAngle;  //车辆轨迹点对应转角（直角坐标系）
    QList<QList<QPointF>> rotateRect;  //车辆轨迹方形坐标点

    double carHistoryLong[6];//历史轨迹经度
    double carHistoryLati[6];//历史轨迹纬度
    double carHistoryHeading[6];//历史轨迹航向角
    bool carHistoryBraking[6];//历史轨迹刹车状态
    QPointF carHistoryPoint[6];//历史轨迹点在当前直角坐标系的坐标

    int CrashNum;       //碰撞点
};
#endif //CARDATA_H
