#include "abstractsensor.h"
#include "Sensor/gps.h"
#include "Sensor/can.h"
#include "Sensor/dsrc.h"

// instantiate template classes

Sensor<GPSData>* GGPS = NULL;
Sensor<CANData>* GCAN = NULL;
Sensor<DSRCData>* GDSRC = NULL;
