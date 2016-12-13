#include "world.h"
#include "Messaging/bsmmessage.h"
#include "Messaging/spatmessage.h"
#include "Messaging/mapmessage.h"
#include <QDebug>

World* GWorld = NULL;
QReadWriteLock warningRWLock;
World::World()
{

}

void World::Initialize()
{
    Self = new SelfVehicle();
    Vehicles[Self->ID] = Self;
}

QList<Vehicle*> World::GetNearbyVehicles(Vehicle* vehicle, double maxDistance)
{
    //TODO: based on a world management data structure (quadtree, octree, bsp etc.)
    return Vehicles.values();
}

void World::UpdateFromBSMMessage(BSMBlob bsmmessage)
{
    if(!Vehicles.contains(bsmmessage.carID))
    {
        Vehicles[bsmmessage.carID] = new Vehicle();
        Vehicles[bsmmessage.carID]->ID = bsmmessage.carID;
        qDebug() << "New vehicle discovered:" << bsmmessage.carID;
    }

    Vehicles[bsmmessage.carID]->UpdateFromBSMMessage(bsmmessage);

    emit onVehicleBSMUpdate(Vehicles[bsmmessage.carID]);
}

inline WorldModel::SignalLightColor ConvertSPATLightStateToWorldModelSignalLightColor(SPATLightState lightState)
{
    switch(lightState)
    {
    case green:
        return WorldModel::SignalLightColor::Green;
    case yellow:
        return WorldModel::SignalLightColor::Yellow;
    case red:
        return WorldModel::SignalLightColor::Red;
    default:
        return WorldModel::SignalLightColor::Error;
    }
}

void World::UpdateFromSPATMessage(SPATIntersectionState spatmessage)
{
    {
        int id = spatmessage.id;
        int lanes = spatmessage.laneCount;
        auto LightStates = spatmessage.states;
        if(this->Intersections.contains(id))
        {
            for(int i = 0; i<LightStates.size() && i < Intersections[id]->approaches.size() ;i++)
            {
                // replicate movementState to all lanes in an approach
                for(auto& lane: Intersections[id]->approaches[i].lanes)
                {
                    // lane.signalLightState = LightStates[0];
                    lane.signalLightState.leftColor = ConvertSPATLightStateToWorldModelSignalLightColor(LightStates[i].leftState);
                    lane.signalLightState.leftTimeToChange = LightStates[i].leftTimeToChange;
                    lane.signalLightState.rightColor = ConvertSPATLightStateToWorldModelSignalLightColor(LightStates[i].rightState);
                    lane.signalLightState.rightTimeToChange = LightStates[i].rightTimeToChange;
                    lane.signalLightState.straightColor = ConvertSPATLightStateToWorldModelSignalLightColor(LightStates[i].straightState);
                    lane.signalLightState.straightTimeToChange = LightStates[i].straightTimeToChange;
                }
            }

         }
    }
}

void World::UpdateFromMAPMessage(MapMsg mapmessage)
{
    for(auto it: mapmessage.intersects)
    {
        // assume that map data won't change quickly, so no updating mechanism here
        if(!this->Intersections.contains(it.id))
        {
            qDebug() << "Update map: intersection " << it.id;
            this->Intersections[it.id] = new WorldModel::Intersection();
            this->Intersections[it.id]->id = it.id;
            this->Intersections[it.id]->center.Latitude = it.center.latitude / 10000000.;
            this->Intersections[it.id]->center.Longitude = it.center.longitude / 10000000.;

//            this->Intersections[it.id]->center.Longitude = 121.2087066;
//            this->Intersections[it.id]->center.Latitude = 31.2913633;

            qDebug() << "Intersection GeoPosition:" << it.center.latitude << it.center.longitude;

            for(int i = 0; i<it.paths.size(); i++)
            {
                auto p = it.paths[i];

                WorldModel::Approach approach;
                approach.junction.Latitude = p.junction.latitude / 10000000.;
                approach.junction.Longitude = p.junction.longitude / 10000000.;
                approach.refLaneWidth = p.lanewidth;

                qDebug() << "  " << p.junction.latitude << p.junction.longitude;
                for(auto l: p.lanes)
                {
                    WorldModel::Lane lane;
                    lane.id = l.laneNumber;
                    lane.width = l.lanewidth;
                    approach.lanes.append(lane);
                }

                // DEBUG PURPOSE ONLY -----------------
                WorldModel::Lane lane;
                lane.id = 0;
                lane.width = 1;
                approach.lanes.append(lane);

                this->Intersections[it.id]->approaches.append(approach);
            }

//            this->Intersections[it.id]->approaches[0].junction.Longitude =121.2087129;
//            this->Intersections[it.id]->approaches[0].junction.Latitude =31.2915010;

//            this->Intersections[it.id]->approaches[1].junction.Longitude =121.2087008;
//            this->Intersections[it.id]->approaches[1].junction.Latitude = 31.2912774;

//            this->Intersections[it.id]->approaches[2].junction.Longitude =121.2088380;
//            this->Intersections[it.id]->approaches[2].junction.Latitude = 31.2913511;

//            this->Intersections[it.id]->approaches[3].junction.Longitude =121.2085772;
//            this->Intersections[it.id]->approaches[3].junction.Latitude = 31.2913610;
        }
    }
}
/****************************************/
QList<WorldModel::Warning_Data*> World::GetWarnings(){
    RefreshWarnings();
    return Warnings.values();
}

void World::RefreshWarnings(){
    for (auto it = Warnings.begin(); it != Warnings.end(); ++it ) {
        WorldModel::Warning_Data* warn=(WorldModel::Warning_Data*)it.value();
        //warningRWLock.lockForWrite();
        if(warn->validCount>0){
            warn->validCount--;
        }else{
            //Warnings.erase(it);
        }
        //warningRWLock.unlock();
    }
}

//void World::updateWarnings(Warning_Data* warn){
//    warningRWLock.lockForWrite();

//    warningRWLock.unlock();
//}

QList<WorldModel::Road_Data*> World::GetRoads(){
    RefreshRoads();
    return Roads.values();
}
void World::RefreshRoads(){
    for (auto it = Roads.begin(); it != Roads.end(); ++it ) {
        WorldModel::Road_Data* road=(WorldModel::Road_Data*)it.value();
        //qDebug("shuchu %d",road->validCount);
        if(road->validCount>0){
            road->validCount--;
        }else{
            //Roads.erase(it);
        }
    }
}
/****************************************/
