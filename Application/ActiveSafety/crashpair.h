#ifndef CRASHPAIR_H
#define CRASHPAIR_H

#include "carData.h"
#include "computetools.h"


class crashPair
{
public:
    crashPair();
    crashPair(carData _selfVehicle,carData _otherVehicle);


    carData otherVehicle;  //其他车辆
    carData selfVehicle;   //自身车辆
    int crashLevel;
    int CollisionInTime; //预计将要发生碰撞的时间
    bool isCollision;
    int CollisionType;  //碰撞变量
    bool BSW; //盲区是否有车
    //生成计算碰撞的数据结构
    void genCrash();

    //车辆之间的碰撞预警
    void CollisionDect_FCW();//前向碰撞预警  代号：1
    void CollisionDect_ICW();//路口交叉碰撞预警  代号：2
    void CollisionDect_BSW_LCW();//盲区及变道预警  代号：3
    void CollisionDect_EEBL();//紧急刹车预警  代号：4
    void CollisionDect_LTA();//路口左转辅助预警  代号：5
    void CollisionDect_DNPW();//逆向超车预警  代号：6

    //人车碰撞预警
    void CollisionDect_CP();//车辆与行人之间的碰撞预警  代号：7


};

#endif // CRASHPAIR_H
