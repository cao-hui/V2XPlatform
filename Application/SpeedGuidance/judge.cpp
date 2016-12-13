#include <QDebug>
#include "Application/SpeedGuidance/judge.h"
#include "UI/mainwindow.h"
#include "v2xplatform.h"
#include "Messaging/spatmessage.h"
#include "WorldModel/worldobject.h"

void Judge_guidance::Initialize()
{
    //trigger_judge();
    connect(GWorld->Self, SIGNAL(onSelfPositioningInfoRefreshed()), SLOT(trigger_judge()), Qt::QueuedConnection);//用于触发整个车速引导
}

extern WorldModel::Approach* light_choose();

void Judge_guidance::trigger_judge()
{

    static SpeedGuidance s1;
    mark_sugestion m1;

    WorldModel::Approach* approach = light_choose();

    if(approach == NULL)
    {
        //mainWindow->Pass_intersection();
        GMainWindow->clear_sGuide();
        return;
    }

    //qDebug()<<"jjjjjjj"<<endl;
    double d1 = s1.Cal_distance(GWorld->Self->Latitude,
                         GWorld->Self->Longitude,
                         approach->junction.Latitude,
                         approach->junction.Longitude);
//qDebug()<<"approach->lanes[0].signalLightState.straightColor"<<approach->lanes[0].signalLightState.straightColor<<endl;

    m1 = s1.Guidance(approach->lanes[0].signalLightState.straightColor,
            approach->lanes[0].signalLightState.straightTimeToChange,
            GWorld->Self->Speed*3.6, 50, d1, approach->lanes[0].signalLightState.straightTime[0],
             approach->lanes[0].signalLightState.straightTime[2]);


    //insert HMIdata
    if(!GWorld->Roads.contains(0)){
        WorldModel::Road_Data *RD=new WorldModel::Road_Data();
        GWorld->Roads[0]=RD;
    }

    GWorld->Roads[0]->validCount=3;
    GWorld->Roads[0]->SignalPhase=m1.Light_status;
    GWorld->Roads[0]->Time_left=m1.Time_left;
    GWorld->Roads[0]->Distance=m1.Distance;
    GWorld->Roads[0]->Curren_speed=m1.Curren_speed;
    GWorld->Roads[0]->Advise_speed=m1.Advise_speed;
    GWorld->Roads[0]->Mark=m1.Mark;

   //qDebug()<<"adas"<<Light_status<<endl;
   //qDebug()<<"ROAD_COUNT: "<<GWorld->Roads.count();

    //mainWindow->Speedguidance_display(m1);
    GMainWindow->refresh_sGuide(m1);
}
