#include "crashpair.h"
#include "UI/mainwindow.h"


crashPair::crashPair()
{

}

crashPair::crashPair(carData _selfVehicle, carData _otherVehicle)
{
    selfVehicle = _selfVehicle;
    otherVehicle = _otherVehicle;
    isCollision = 0;
    genCrash();
}


//生成计算碰撞的数据结构
void crashPair::genCrash()
{
    //坐标转换（以自车为坐标原点，将GPS坐标转换为直角坐标，
    //并将他车的位置标出在本车的坐标系里面)
    GPStoRecSys(selfVehicle,otherVehicle);

    CenterPointPre(selfVehicle);
    CenterPointPre(otherVehicle);
//    MovePoint_Present(selfVehicle);
//    MovePoint_Present(otherVehicle);

// 场景类型
// 车辆之间的碰撞
// 程序里面：AtCross, LeftTurnLight,BrakeStatus 还没有提供
    if(otherVehicle.VID.toInt() < 100)
    {
        if(selfVehicle.AtCross == true && otherVehicle.AtCross ==true)
        {
            if(selfVehicle.LeftTurnLight == true)
            {
                CollisionDect_LTA();
            }
            else
            {
                CollisionDect_ICW();
            }
        }
        else
        {
            if(otherVehicle.BrakeStatus == true)
            {
                CollisionDect_EEBL();
            }
            else
            {
                if(selfVehicle.LeftTurnLight == true)
                {
                    if(qFabs(selfVehicle.heading_Axis-otherVehicle.heading_Axis)>90)
                    {

                        CollisionDect_DNPW();
                    }
                    else
                    {
                        CollisionDect_BSW_LCW();
                    }
                }
                else
                {
                    CollisionDect_FCW();
                }
            }
        }
    }

    //车辆和行人之间的碰撞
    else
    {
        CollisionDect_CP();
    }
}



void crashPair::CollisionDect_FCW()
{
    int n;
    double A; //直角坐标的航向角度差
    double r;
    A = otherVehicle.heading_Axis-selfVehicle.heading_Axis;
    r = selfVehicle.vehicleWheelDis/(qSin(selfVehicle.wheelAngle*PI/180)+0.000001);//转弯半径
    if(selfVehicle.Speedmps > otherVehicle.Speedmps)
    {
        n=1+(int)A*r/((selfVehicle.Speedmps-otherVehicle.Speedmps)*0.02);
        selfVehicle.CrashNum = n;
        otherVehicle.CrashNum = n;
        if(selfVehicle.Speedmps > 0.3)
        {
            isCollision = true;
        }
    }
}

void crashPair::CollisionDect_ICW()
{
    double d0;//碰撞的临界距离
    double d1;
    double d;//实际距离

    for(int i=1;i<selfVehicle.rotateAngle.count();i++)
    {
        d1 = qSqrt((selfVehicle.vehicleLength/2.0)*(selfVehicle.vehicleLength/2.0)
                   +(otherVehicle.vehicleLength/2.0)*(otherVehicle.vehicleLength/2.0)
                   +selfVehicle.vehicleLength*otherVehicle.vehicleLength/2.0
                   *qCos(qFabs(selfVehicle.rotateAngle[i]-otherVehicle.rotateAngle[i])));

        d0 = qSqrt((selfVehicle.vehicleWidth/2.0)*(selfVehicle.vehicleWidth/2.0)
                  +(otherVehicle.vehicleWidth/2.0)*(otherVehicle.vehicleWidth/2.0)
                  +selfVehicle.vehicleWidth*otherVehicle.vehicleWidth/2.0
                  *qCos(qFabs(selfVehicle.rotateAngle[i]-otherVehicle.rotateAngle[i])));
        d0 = d1 + d1;
        d = qSqrt((selfVehicle.carLine.at(i).x()-otherVehicle.carLine.at(i).x())*
                  (selfVehicle.carLine.at(i).x()-otherVehicle.carLine.at(i).x())
                  +(selfVehicle.carLine.at(i).y()-otherVehicle.carLine.at(i).y())*
                  (selfVehicle.carLine.at(i).y()-otherVehicle.carLine.at(i).y()));
        if(d < d0)
        {
            selfVehicle.CrashNum = i;
            otherVehicle.CrashNum = i;
            if(selfVehicle.Speedmps>0.3 || otherVehicle.Speedmps>0.3)
            {
                isCollision = true;
                break;
            }
        }
    }
}


void crashPair::CollisionDect_BSW_LCW()
{
    double l1;
    double l2;
    double l3;

    double x;
    double y;
    double w;
    double l;

    l1 = qSqrt((selfVehicle.startPoint.x()-otherVehicle.startPoint.x())*
               (selfVehicle.startPoint.x()- otherVehicle.startPoint.x())+
               (selfVehicle.startPoint.y()-otherVehicle.startPoint.y())*
               (selfVehicle.startPoint.y()-otherVehicle.startPoint.y()));
    l3 = ((selfVehicle.startPoint.x()-otherVehicle.startPoint.x())+
            (selfVehicle.startPoint.y()-otherVehicle.startPoint.y())*
            qTan(otherVehicle.heading_Axis))/qSqrt(1+qTan(otherVehicle.heading_Axis)*qTan(otherVehicle.heading_Axis));
    l2 = qSqrt(l1*l1 - l3*l3);
    if(l2 < (selfVehicle.vehicleWidth+otherVehicle.vehicleWidth+1)/2 && l3 < (selfVehicle.vehicleLength+otherVehicle.vehicleLength)/2)
    {
        BSW = true; //盲区有车   
    }

    w = (selfVehicle.vehicleWidth+otherVehicle.vehicleWidth)/2.0;
    l = (selfVehicle.vehicleLength+otherVehicle.vehicleLength)/2.0;
    for(int i= 0;i<selfVehicle.rotateAngle.count();i++)
    {
        x = selfVehicle.carLine.at(i).x()-otherVehicle.carLine.at(i).x();
        y = selfVehicle.carLine.at(i).y()-otherVehicle.carLine.at(i).y();
        if(x<=w && y<=l)
        {
            if(selfVehicle.Speedmps > 0.3 || otherVehicle.Speedmps>0.3)
            {
                selfVehicle.CrashNum = i;
                otherVehicle.CrashNum = i;
                isCollision = true;
                break;
            }
        }
    }
}

void crashPair::CollisionDect_EEBL()
{
    double dw;//临界的距离
    double dl;
    double d;//两车之间的实时距离
    double r; //自车的转弯半径
    double x0;//自车的轨迹圆心
    double y0;

    r = selfVehicle.vehicleWheelDis/(qSin(selfVehicle.wheelAngle*PI/180)+0.000001);//转弯半径
    x0 = selfVehicle.startPoint.x()+r*qCos(selfVehicle.heading_Axis);
    y0 = selfVehicle.startPoint.y()+r*qSin(selfVehicle.heading_Axis);
    dw = (selfVehicle.vehicleWidth+otherVehicle.vehicleWidth)/2.0;
    dl = (selfVehicle.vehicleLength+otherVehicle.vehicleLength)/2.0;

    //自车轨迹圆(x-x0)*(x-x0)+(y-y0)*(y-y0)=r*r;
    if((otherVehicle.startPoint.x()-x0)*(otherVehicle.startPoint.x()-x0)+
       (otherVehicle.startPoint.y()-y0)*(otherVehicle.startPoint.y()-y0)-r*r<(dw*dw))
    {
        for(int i=0;i<selfVehicle.rotateAngle.count();i++)
        {
            d = qSqrt((selfVehicle.carLine.at(i).x()-otherVehicle.startPoint.x())*
                      (selfVehicle.carLine.at(i).x()-otherVehicle.startPoint.x())
                          +(selfVehicle.carLine.at(i).y()-otherVehicle.startPoint.y())*
                      (selfVehicle.carLine.at(i).y()-otherVehicle.startPoint.y()));
            if(d<dl && selfVehicle.Speedmps > 0.3)
            {
                isCollision = true;
                selfVehicle.CrashNum = i;
                otherVehicle.CrashNum = i;
                break;
            }
        }
    }
}

void crashPair::CollisionDect_LTA()
{
    double x;
    double y;
    double w;
    double l;
    w = (selfVehicle.vehicleWidth+otherVehicle.vehicleWidth)/2.0;
    l = (selfVehicle.vehicleLength+otherVehicle.vehicleLength)/2.0;
    for(int i= 0;i<selfVehicle.rotateAngle.count();i++)
    {
        x = selfVehicle.carLine.at(i).x()-otherVehicle.carLine.at(i).x();
        y = selfVehicle.carLine.at(i).y()-otherVehicle.carLine.at(i).y();
        if(x<=w && y<=l)
        {
            if(selfVehicle.Speedmps > 0.3 || otherVehicle.Speedmps>0.3)
            {
                selfVehicle.CrashNum = i;
                otherVehicle.CrashNum = i;
                isCollision = true;
                break;
            }
        }
    }
}

void crashPair::CollisionDect_DNPW()
{
    double d0; //碰撞的临界距离
    double d; //实际距离
    d0 = qSqrt((selfVehicle.vehicleWidth/2.0+otherVehicle.vehicleWidth/2.0)
              *(selfVehicle.vehicleWidth/2.0+otherVehicle.vehicleWidth/2.0)
              +(selfVehicle.vehicleLength/2.0+otherVehicle.vehicleLength/2.0)
              *(selfVehicle.vehicleLength/2.0+otherVehicle.vehicleLength/2.0));

    for(int i=1;i<selfVehicle.rotateAngle.count();i++)
    {
        d = qSqrt((selfVehicle.carLine.at(i).x()-otherVehicle.carLine.at(i).x())
                  *(selfVehicle.carLine.at(i).x()-otherVehicle.carLine.at(i).x())
                  +(selfVehicle.carLine.at(i).y()-otherVehicle.carLine.at(i).y())
                  *(selfVehicle.carLine.at(i).y()-otherVehicle.carLine.at(i).y()));

        if(d < d0)
        {
            selfVehicle.CrashNum = i;
            otherVehicle.CrashNum = i;
            if(selfVehicle.Speedmps>0.3 || otherVehicle.Speedmps>0.3)
            {
                isCollision = true;
                break;
            }
        }
    }
}

void crashPair::CollisionDect_CP()
{
    int R = 5;
    for(int i=0;i<selfVehicle.rotateAngle.count();i++)
    {
        if(qSqrt((selfVehicle.carLine.at(i).x()-otherVehicle.carLine.at(i).x())*
                 (selfVehicle.carLine.at(i).x()-otherVehicle.carLine.at(i).x())
                 + (selfVehicle.carLine.at(i).y()-otherVehicle.carLine.at(i).x())*
                 (selfVehicle.carLine.at(i).y()-otherVehicle.carLine.at(i).y())) < R)
        {
            if(selfVehicle.Speedmps > 0.3)
            {
                selfVehicle.CrashNum = i;
                otherVehicle.CrashNum = i;
                isCollision = true;
                break;
            }
        }
    }
}









