#include "carData.h"
#include "crashpair.h"
#include "activesafety.h"
#include "UI/mainwindow.h"
#include "v2xplatform.h"
#include <QDebug>
#include "WorldModel/worldobject.h"

void ActiveSafety::Initialize()
{
    connect(GWorld, SIGNAL(onVehicleBSMUpdate(Vehicle*)), SLOT(onVehicleBSMUpdate(Vehicle*)));
    connect(GWorld->Self, SIGNAL(onSelfPositioningInfoRefreshed()), SLOT(onSelfVehicleUpdate()));
}

void ActiveSafety::onSelfVehicleUpdate()
{
    onVehicleBSMUpdate(NULL);
}

void ActiveSafety::onVehicleBSMUpdate(Vehicle *vehicle)
{
    GMainWindow->refresh_rawData();
    carData self(QString::number(GWorld->Self->ID), true, GWorld->Self->Longitude, GWorld->Self->Latitude, GWorld->Self->Speed,
                 GWorld->Self->Heading, GWorld->Self->WheelAngle/13,GWorld->Self->IsBraking);
    self.vehicleWidth = 1.551;
    self.vehicleLength = 3.569;
    self.vehicleWheelDis = 2.569;

    // 历史轨迹的填充,当前点也属于历史轨迹,
    for(int i=0;i<5;i++)
    {
        HistoryLong[i] = HistoryLong[i+1];
        HistoryLati[i] = HistoryLati[i+1];
        HistoryHeading[i] = HistoryHeading[i+1];
        HistoryBraking[i] = HistoryBraking[i+1];
        self.carHistoryLong[i] = HistoryLong[i];
        self.carHistoryLati[i] = HistoryLati[i];
        self.carHistoryHeading[i] = HistoryHeading[i];
        self.carHistoryBraking[i] = HistoryBraking[i];
    }
    HistoryLong[5] = self.VLongitude;
    HistoryLati[5] = self.VLatitude;
    HistoryHeading[5] = self.wheelAngle;
    HistoryBraking[5] = self.BrakeStatus;
    self.carHistoryLong[5] = HistoryLong[5];
    self.carHistoryLati[5] = HistoryLati[5];
    self.carHistoryHeading[5] = HistoryHeading[5];
    self.carHistoryBraking[5] = HistoryBraking[5];

    //在这里发送BSM消息

    //mainWindow->refreshSelfData(self);

    //Generate crashPair data for algorithm
    auto allcars = GWorld->GetNearbyVehicles(GWorld->Self);
    for(auto it: allcars)
    {
        if(it->ID == GWorld->Self->ID) continue;

        carData other(QString::number(it->ID), true, it->Longitude, it->Latitude,
                      it->Speed, it->Heading, it->WheelAngle/13,it->IsBraking);
        other.vehicleWidth = 1.551;
        other.vehicleLength = 3.569;
        other.vehicleWheelDis = 2.569;

        crashPair pair(self, other); //此处进行历史轨迹

        GMainWindow->addObjectsToMap(pair.selfVehicle.VID.toInt(),pair.selfVehicle.carLine);
        GMainWindow->addObjectsToMap(pair.otherVehicle.VID.toInt(),pair.otherVehicle.carLine);

        //insert HMI
        //heading 2=车辆  4=行人
        if(pair.isCollision == true)
        {
            if(!GWorld->Warnings.contains(it->ID))
            {
                WorldModel::Warning_Data* warnData=new WorldModel::Warning_Data();
                GWorld->Warnings[it->ID]=(warnData);
            }
            GWorld->Warnings[it->ID]->validCount = 3;
            if(other.VID.toInt() < 100)
            {
                GWorld->Warnings[it->ID]->WarningStr = 0;
            }
            else if(other.VID.toInt() >= 100)
            {
                if(other.heading_GPS == 2){
                    GWorld->Warnings[it->ID]->WarningStr = 0;}
                if(other.heading_GPS == 4){
                    GWorld->Warnings[it->ID]->WarningStr = 4;}
            }

            GWorld->Warnings[it->ID]->WarningLevel = pair.crashLevel;
            GWorld->Warnings[it->ID]->WarningID = QString::number(it->ID);
            //GWorld->Warnings.insert(it->ID,warnData);
        }
        //qDebug()<<"WARNING_COUNT: "<<GWorld->Warnings.count();

        //mainWindow->refreshTestUI(pair);
    }
}
