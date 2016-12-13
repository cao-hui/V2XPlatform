#include "carData.h"

carData::carData()
{

}


carData::carData(QString p_VID, bool p_selfvehicle, double p_vLongitude, double p_vLatitude,
                 double p_speedmps, double p_heading_gps, double p_wheelangle)
                // )
{
    VID = p_VID;

    ifSelfVehicle = p_selfvehicle;

    VLongitude = p_vLongitude;
    VLatitude = p_vLatitude;
    Speedmps = p_speedmps;
    heading_GPS = p_heading_gps;
    wheelAngle = p_wheelangle;
    CrashNum = -1;
}

carData::carData(QString p_VID, bool p_selfvehicle, double p_vLongitude, double p_vLatitude,
                 double p_speedmps, double p_heading_gps, double p_wheelangle,bool p_IsBraking)
                // bool p_LeftTurnLight,bool p_AtCross,bool p_BrakeStatus)
{
    VID = p_VID;

    ifSelfVehicle = p_selfvehicle;

    VLongitude = p_vLongitude;
    VLatitude = p_vLatitude;
    Speedmps = p_speedmps;
    heading_GPS = p_heading_gps;
    wheelAngle = p_wheelangle;
    CrashNum = -1;

    BrakeStatus = p_IsBraking;
//    AtCross = p_AtCross;
//    LeftTurnLight = p_LeftTurnLight;
}
