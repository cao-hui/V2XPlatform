#include "WorldModel/world.h"
#include <QDebug>

#define PI 3.1415926535

struct Vector2D
{
    double x;
    double y;
};  //向量

Vector2D Vector2DFromTwoPoints(double lat1, double lng1, double lat2, double lng2)
{
    lat1 = lat1/180*PI;
    lng1 = lng1/180*PI;
    lat2 = lat2/180*PI;
    lng2 = lng2/180*PI;
    Vector2D p;
    p.y = 6371004*(lat2-lat1);
    p.x = 6371004*cos(lat1)*(lng2-lng1);
    return p;
}

double CalculateDistance(double lat1, double lng1, double lat2, double lng2)
{
    lat1 = lat1/180*PI;
    lng1 = lng1/180*PI;
    lat2 = lat2/180*PI;
    lng2 = lng2/180*PI;
    double y = 6371004*(lat2-lat1);
    double x = 6371004*cos(lat1)*(lng2-lng1);
    return sqrt(x*x+y*y);
}

WorldModel::Approach* light_choose()            //每次有路口消息时调用该函数
{

    for(auto& intersection: GWorld->Intersections)
    {

        //qDebug()<<"juli"<<intersection->center.Latitude<<endl;
        //qDebug()<<"jingdu"<<intersection->center.Longitude<<endl;
        double distance = CalculateDistance(GWorld->Self->Latitude,
                                  GWorld->Self->Longitude,
                                  intersection->center.Latitude,
                                  intersection->center.Longitude);//计算车辆与路口的距离
        //qDebug()<<"juli"<<distance<<endl;
        if(distance < 300)
        {
            Vector2D crossPoint = Vector2DFromTwoPoints(GWorld->Self->Latitude,
                                           GWorld->Self->Longitude,
                                           intersection->center.Latitude,
                                           intersection->center.Longitude);    //车到路口的向量

            double cosAngle;
            double vectorLong;
            vectorLong = fabs(sqrt(crossPoint.x * crossPoint.x + crossPoint.y * crossPoint.y));
            cosAngle = (crossPoint.x*cos((90-GWorld->Self->Heading)/180*PI)+crossPoint.y*sin((90-GWorld->Self->Heading)/180*PI)) / vectorLong;
            //qDebug()<<"jiaodu"<<cosAngle<<endl;
            if(cosAngle>0.8 && cosAngle<=1)   //车辆航向角与车到路口间向量的夹角是否小于20°
            {
                Vector2D lightPoint;
                double maxProduct = -999999;
                WorldModel::Approach* maxApproach = NULL;
                //qDebug() << "----";
                for(int i = 0; i < intersection->approaches.size(); i++)
                {
                    //qDebug()<<"ssss:"<<intersection->approaches.size()<<endl;
                    WorldModel::Approach* approach = &intersection->approaches[i];

                    lightPoint = Vector2DFromTwoPoints(
                                approach->junction.Latitude,
                                approach->junction.Longitude,
                                intersection->center.Latitude,
                                intersection->center.Longitude); //路口交叉点->路口中心的向量

                    double vectorProduct = lightPoint.x * crossPoint.x + lightPoint.y * crossPoint.y;   //两个向量的积

//                    Vector2D lightVehicle = Vector2DFromTwoPoints(
//                                approach->junction.Latitude,
//                                approach->junction.Longitude,
//                                GWorld->Self->Latitude,
//                                GWorld->Self->Longitude);

//                    double isPassed = lightVehicle.x *cos((90-GWorld->Self->Heading)/180*PI) + lightVehicle.y*sin((90-GWorld->Self->Heading)/180*PI);
//                    qDebug() << isPassed;
//                    if(isPassed > 0)
//                        continue;

                    if(vectorProduct > maxProduct)
                    {
                        maxProduct = vectorProduct;
                        maxApproach = approach;
                    }
                }

                Vector2D lightVehicle = Vector2DFromTwoPoints(
                                    maxApproach->junction.Latitude,
                                    maxApproach->junction.Longitude,
                                    GWorld->Self->Latitude,
                                    GWorld->Self->Longitude);
                //qDebug("self Lat %f  %f",GWorld->Self->Latitude,GWorld->Self->Longitude);
                Vector2D lightcross=Vector2DFromTwoPoints(
                            intersection->center.Latitude,
                            intersection->center.Longitude,
                            maxApproach->junction.Latitude,
                            maxApproach->junction.Longitude);
                //在遇到第一个满足20°条件的路口时即返回
                //qDebug()<<"xuanzhong"<<maxApproach->junction.Latitude<<"   "<<maxApproach->junction.Longitude<<endl;
                double isPassed = lightVehicle.x *lightcross.x+lightVehicle.y*lightcross.y;
                //qDebug()<<"ispass"<<isPassed<<endl;
                if(isPassed>0)
                    return maxApproach;
            }
        }
        else {
            //qDebug() << "Intersection" << intersection->id << "is too far away (>100m)";
        }
    }

    return NULL;
}





