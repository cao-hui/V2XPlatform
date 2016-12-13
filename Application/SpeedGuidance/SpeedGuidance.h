#ifndef SpeedGuidance_H
#define SpeedGuidance_H

#include "math.h"
#include "Fusion/positioning.h"

struct mark_sugestion
{
    int Light_status;
    int Time_left;
    int Advise_speed;
    int Curren_speed;
    int Mark;
    int Distance;
};


class SpeedGuidance
{
    public:
        SpeedGuidance();
        mark_sugestion Guidance(int light_status,int lefttime,double current_speed,int limit_speed,double distance_toIS,int greenlight_time,int redlight_time);//引导函数+display
        int Intersection(double distanceto_Intersection,double intersection_circle,int current_intersectionID);//判断是否需要引导
        double Cal_distance(double latitude1,double longitude1,double latitude2,double longitude2);//计算距离

    private:
        int Lastintersection_ID;//存放已过路口ID
};

#endif // SpeedGuidance_H
