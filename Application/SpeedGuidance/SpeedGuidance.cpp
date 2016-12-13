#include "SpeedGuidance.h"
#include "UI/mainwindow.h"
#include "QDebug"


#define pi 3.1415926

SpeedGuidance::SpeedGuidance()
{
    Lastintersection_ID=0;
}

/*
定义距离计算函数：此距离是路口中心到路口的距离
用于判断车辆进入路口区
latitude1,longitude1,latitude2,longitude2是路口中心点和一个红绿灯的坐标；
用于计算两个点之间的距离
*/
double SpeedGuidance::Cal_distance(double latitude1,double longitude1,double latitude2,double longitude2)
{
    double distance;
    distance=0.0;
    int Earth_R;
    Earth_R=6371004;
    distance=Earth_R*acos(sin(latitude1*pi/180)*sin(latitude2*pi/180)*cos((longitude1-longitude2)*pi/180)+cos(latitude1*pi/180)*cos(latitude2*pi/180));
    return distance;
}
/*
定义进入路口判断函数， back 1：需要引导; 0：不需要引导
数据：L,R
*/
int SpeedGuidance:: Intersection(double distanceto_Intersection,double intersection_circle,int current_intersectionID)//主程序中需要最LastID做第一次赋值
{
    int value;
    value=0;
    if(distanceto_Intersection<=intersection_circle)
    {
        Lastintersection_ID=current_intersectionID;
        value=0;
    }
    else
    {
        if(Lastintersection_ID!=current_intersectionID)
        {
            //引导
            value=1;
        }
    }
    return value;
}



/*
定义引导函数；
light_status信号灯状态:0代表绿灯,1代表红灯
lefttime剩余时间
current_speed当前速度
limit_speed道路限制速度
distance_toIS车辆到路口距离
greenlight_time信号灯周期中绿灯时间
ID：路口ID
*/
mark_sugestion SpeedGuidance::Guidance(int light_status,int lefttime,double current_speed,int limit_speed,double distance_toIS,int greenlight_time,int redlight_time)
{
    double min_speed;
    min_speed=0.0;
    double max_speed;
    max_speed=0.0;
    double differ_speed;
    differ_speed=0.0;
    int advise_speed;
    advise_speed=0;

    mark_sugestion mark;
    mark.Light_status=light_status;
    mark.Time_left=lefttime;
    mark.Curren_speed=current_speed;
    mark.Distance=distance_toIS;
    if(light_status==3)//绿灯情况进行判断
    {
        min_speed=distance_toIS/lefttime;
        min_speed=min_speed*3.6;//转换速度单位：m/s到km/h
        if(min_speed>limit_speed)
        {
            mark.Mark=0;
            mark.Advise_speed=0;
        }
        else
        {
            advise_speed=(min_speed+limit_speed)/2;//建议车速
            mark.Advise_speed=advise_speed;
            if((current_speed>min_speed)&&(current_speed<limit_speed))
            {
                mark.Mark=1;
               // qDebug()<<advise_speed;
            }
            else if(current_speed<=min_speed)
            {
                differ_speed=abs(advise_speed-current_speed);
                if(differ_speed<10)
                {
                    mark.Mark=2;
                    //qDebug()<<advise_speed;
                }
                else
                {
                    mark.Mark=3;
                    //qDebug()<<advise_speed;
                }
            }
            else if(current_speed>=limit_speed)
            {
                differ_speed=abs(current_speed-current_speed);
                if(differ_speed<10)
                {
                    mark.Mark=4;
                    //qDebug()<<advise_speed;
                }
                else
                {
                    mark.Mark=5;
                   // qDebug()<<advise_speed;
                }
            }
        }
    }
    else
    {
        if(light_status==2)
        {
            min_speed=distance_toIS/(lefttime+greenlight_time+redlight_time);
            max_speed=distance_toIS/(lefttime+redlight_time);
        }

        else
        {
            min_speed=distance_toIS/(lefttime+greenlight_time);//最小速度,绿灯红灯时间必须已知
            max_speed=distance_toIS/lefttime;//最大速度
        }
                min_speed=min_speed*3.6;
                max_speed=max_speed*3.6;
                if(max_speed>limit_speed)
                {
                    advise_speed=(min_speed+limit_speed)/2;
                    mark.Advise_speed=advise_speed;
                    if(advise_speed<10)
                    {
                        mark.Mark=6;
                        mark.Advise_speed=0;
                    }
                    else
                    {
                        if((current_speed>min_speed)&&(current_speed<limit_speed))
                        {
                            mark.Mark=7;
                        }
                        else if(current_speed<=min_speed)
                        {
                            differ_speed=abs(advise_speed-current_speed);
                            if(differ_speed<10)
                            {
                                mark.Mark=8;
                            }
                            else
                            {
                                mark.Mark=9;
                            }
                        }
                        else if(current_speed>=limit_speed)
                        {
                            differ_speed=abs(current_speed-advise_speed);
                            if(differ_speed<10)
                            {
                                mark.Mark=10;
                            }
                            else
                            {
                                mark.Mark=11;
                            }
                        }
                    }
                }
                else
                {
                    advise_speed=(min_speed+max_speed)/2;
                    mark.Advise_speed=advise_speed;
                    if(advise_speed<10)
                    {
                        mark.Mark=12;
                        mark.Advise_speed=0;
                    }
                    else
                    {
                        if((current_speed>min_speed)&&(current_speed<max_speed))
                        {
                            mark.Mark=13;
                        }
                        else if(current_speed<=min_speed)
                        {
                            differ_speed=abs(advise_speed-current_speed);
                            if(differ_speed<10)
                            {
                                mark.Mark=14;
                            }
                            else
                            {
                                mark.Mark=15;
                            }
                        }
                        else if(current_speed>=max_speed)
                        {
                            differ_speed=abs(current_speed-advise_speed)/2;
                            if(differ_speed<10)
                            {   
                                mark.Mark=16;
                            }
                            else
                            {
                                mark.Mark=17;
                            }
                        }
                    }
                }
    }
    return mark;
}
