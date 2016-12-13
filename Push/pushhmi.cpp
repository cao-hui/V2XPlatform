#include "pushhmi.h"

PushHmi::PushHmi(QObject *parent) : QObject(parent)
{
    timer=new QTimer();
    ipAddr="192.168.0.4";
    port=4040;
    connect(timer,SIGNAL(timeout()),this,SLOT(sendUdp_show()));
}

PushHmi::~PushHmi()
{
    timer->stop();
    delete timer;
    fclose(ra);
}

void PushHmi::init(QString ipAddr,int port){
    this->ipAddr=ipAddr;
    this->port=port;
    ra =fopen("HMI_Data_0606_V01.txt","w");
}
void PushHmi::startPushing(){
    timer->start(100);
}

//@2016/05/31 原**_Data 改为**_Info; 例 Vehicle_Data Vehicle_Info
void PushHmi::sendUdp_show(){
    QJsonObject json;
    json.insert("Header", QString("SAIC"));
    json.insert("UserID", QString::number( (GWorld->Self)->ID ));
    json.insert("RoadType", 1);
    json.insert("Dispalymode", 2);
    json.insert("MapMode", 1);
    json.insert("DSRCMode", 1);
    json.insert("Length", 0);

    json.insert("Vehicle_Info" , updateVehicle_Data());
    json.insert("Road_Info" , updateRoad_Data());
    json.insert("Even_Info" , updateEvent_Data());//这儿Even_Info没有错,原定义如此 //目前貌似没有Even事件
    json.insert("Warning_Info" , updateWarning_Data());

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray byte_array = doc.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);

    //fprintf(ra,"%s\n\n" ,json_str);
    //qDebug()<<json_str<<endl;

    QUdpSocket* udp=new QUdpSocket();
    udp->writeDatagram(byte_array,QHostAddress(ipAddr),(quint16)port);
    udp->close();
    delete udp;
}

/**
  * Vehicle_Data
  * @2016/05/31 dx、dy分别表示相对自车前方多少米、左方多少米，以下做了改动
  * */
QJsonArray PushHmi::updateVehicle_Data(){
    QJsonArray vData;

    QList<Vehicle*> vList = GWorld->GetNearbyVehicles(GWorld->Self);
    //Vehicle selfVehicle = *((Vehicle*)GWorld->Self);
    int length=vList.size();

    //
    {
        QJsonObject car;
        car.insert("HostVehicle", true);
        car.insert("ID", QString::number(GWorld->Self->ID));
        car.insert("Type",QString("Car"));
        car.insert("dx", 0);
        car.insert("dy", 0);
        car.insert("Angel", GWorld->Self->Heading);
        car.insert("Speed", GWorld->Self->Speed);
        car.insert("Status", 0);
        car.insert("attr1", 1);
        vData.insert(0 , car);
    }
    int count=1;
    for (int i = 0; i < length; i++){
        QJsonObject car;
        car.insert("HostVehicle", (vList[i]->ID == GWorld->Self->ID));
        car.insert("ID", QString::number(vList[i]->ID));
        car.insert("Type",QString("Car"));
        if((vList[i]->ID == GWorld->Self->ID)){
            car.insert("dx", 0);
            car.insert("dy", 0);
            continue;
        }else{
            //经纬度系数
//            double LatiRatio = 111712.69150641056;   //6371004*PI/180
//            double LonRatio = LatiRatio*cos(GWorld->Self->Latitude/180.0*3.1415926535897931);
//            //对其他车辆坐标进行转换
//            double xx = (double) ((vList[i]->Longitude  - GWorld->Self->Longitude) * LonRatio);
//            double yy = (double) ((vList[i]->Latitude - GWorld->Self->Latitude) * LatiRatio);
//            //前方?米 ; 左方?米
//            //v= ( cosα, sinα ) ;p=( x, y )
//            double anglejia = 90 - GWorld->Self->Heading;//由正北 0 正东90 正南180 正西270 --> 正东0 正北90 正西180 正南270
//            anglejia = anglejia/180.0*3.1415926535897931;//
//            qDebug()<<"JIA_head"<<xx*cos(anglejia) + yy*sin(anglejia);
//            qDebug()<<"JIA_left"<<-xx*sin(anglejia) + yy*cos(anglejia);

            /************************************************/
            carData selfVehicle(QString("Self"), true, GWorld->Self->Longitude, GWorld->Self->Latitude, GWorld->Self->Speed, GWorld->Self->Heading, GWorld->Self->WheelAngle/13);
            carData otherVehicle(QString("Other"), true, vList[i]->Longitude, vList[i]->Latitude, vList[i]->Speed, vList[i]->Heading, vList[i]->WheelAngle/13);

            GPStoRecSys(selfVehicle,otherVehicle);

            //航向角标定
            float rotateAngle = selfVehicle.heading_GPS;   //旋转角度

            if((otherVehicle.heading_GPS - selfVehicle.heading_GPS)<0)
            {
                otherVehicle.heading_GPS = (otherVehicle.heading_GPS - selfVehicle.heading_GPS) + 360;
            }
            else
            {
                otherVehicle.heading_GPS = otherVehicle.heading_GPS - selfVehicle.heading_GPS;
            }
            selfVehicle.heading_GPS = 0;
            //坐标标定,弧度坐标系
            float x = otherVehicle.startPoint.x();
            float y = otherVehicle.startPoint.y();
            //qDebug("初始坐标：%f\t%f",x,y);
            float r = qSqrt(x*x + y*y);
            float angle = ((y>0)?qAcos(x/r):(-qAcos(x/r)))/PI*180;
            //qDebug("坐标角度：\t%f",angle);
            float newAngle = ((angle+rotateAngle)>180?(angle+rotateAngle-360):(angle+rotateAngle))/180*PI;
//            otherVehicle.startPoint.setX(r*qCos(newAngle));
//            otherVehicle.startPoint.setY(r*qSin(newAngle));
            double left = -r*qCos(newAngle);
            double ahead = r*qSin(newAngle);

            //fprintf(ra,"left:%f\t\t\tahead:%f\n" ,left,ahead);

            //qDebug()<<"HMIGPS: "<<left<<"   "<<ahead;

            car.insert("dx", ahead);
            car.insert("dy", left);
        }
        car.insert("Angel", vList[i]->Heading);
        car.insert("Speed", vList[i]->Speed);
        car.insert("Status", 0);
        car.insert("attr1", 1);
        vData.insert(count , car);
        count++;
    }
    for(int i=length;i<9;i++){
        QJsonObject car;
        car.insert("HostVehicle", false);
        car.insert("ID", QString(""));
        car.insert("Type",QString("Car"));
        car.insert("dx", 0);
        car.insert("dy", 0);
        car.insert("Angel", 0);
        car.insert("Speed", 0);
        car.insert("Status", 0);
        car.insert("attr1", 0);
        vData.insert(i , car);
    }

    QList<QString> Json_VD;
    for(int i=0;i<vData.count();i++)
    {
        //qDebug()<<convertQJsontoQString(vData.at(i).toObject())<<"\n";
        Json_VD.append(convertQJsontoQString(vData.at(i).toObject()));
    }
    //mainWindow->show_Json_VD(Json_VD);

    return vData;
}
/**
  * Road_Data
  * 主要是信号灯信息
  * HMI收到信号灯信息会马上显示
  * */

QJsonArray PushHmi::updateRoad_Data(){

    QList<WorldModel::Road_Data*> rList = GWorld->GetRoads();
    QJsonArray rData;
    int count=0;

    for (int i = 0; i < rList.size(); i++){
        if(rList[i]->validCount>0){
            QJsonObject obj;
            obj.insert("DeviceID", 1);
            obj.insert("Distance", rList[i]->Distance);
            obj.insert("SignalPhase", rList[i]->SignalPhase);
            obj.insert("RemainTime", rList[i]->Time_left);
            obj.insert("Advice", rList[i]->Mark);
            obj.insert("AdviceSpeed", rList[i]->Advise_speed);
            obj.insert("att1", 0);
            rData.insert(count , obj);
            count++;
        }
    }
    for(int i=count;i<3;i++){
        QJsonObject obj;
        obj.insert("DeviceID", 0);
        obj.insert("Distance", 0);
        obj.insert("SignalPhase", 0);
        obj.insert("RemainTime", 0);
        obj.insert("Advice", 0);
        obj.insert("AdviceSpeed", 0);
        obj.insert("att1", 0);
        rData.insert(i , obj);
    }


    QList<QString> Json_RD;
    for(int i=0;i<rData.count();i++)
    {
        Json_RD.append(convertQJsontoQString(rData.at(i).toObject()));
    }
    //mainWindow->show_Json_RD(Json_RD);

    return rData;
}

/**
  * Event_Data
  * 车辆事件:包括车辆故障、紧急刹车、公交车入站、公交车驻站、公交车出站
  * 路侧事件:包括道路施工、交通事故、路面湿滑等
  * 行人/动物事件
  * */
QJsonArray PushHmi::updateEvent_Data(){
    //TODO 目前貌似没有这类报警
    QJsonArray eData;

    for (int i = 0; i < 6; i++){
        QJsonObject obj;
        obj.insert("EventID", 0);
        obj.insert("EventName", 0);
        obj.insert("EventType", 0);
        obj.insert("dx1", 0);
        obj.insert("dy1", 0);
        obj.insert("dx2", 0);
        obj.insert("dy2", 0);
        obj.insert("att1", 0);
        eData.insert(i , obj);
    }
    return eData;
}

/**
  * Warning_Data
  * 车车碰撞预警
  * 车人/非机动车碰撞预警
  * 车路预警:包括闯红灯预警、超速预警等
  * */
QJsonArray PushHmi::updateWarning_Data(){

    QList<WorldModel::Warning_Data*> wList = GWorld->GetWarnings();
    QJsonArray wData;

    int count=0;
    for (int i = 0; i < wList.size(); i++){
        if(wList[i]->validCount>0){
            QJsonObject obj;
            obj.insert("WarningID", (wList[i]->WarningID));
            obj.insert("WarningStr", (wList[i]->WarningStr));
            obj.insert("WarningLevel", 1);//wList[i]->WarningLevel= 1 ;WarningStr = 0 对应碰撞报警
            obj.insert("att1", 0);
            wData.insert(count , obj);
            count++;
        }
    }
    for(int i=count;i<4;i++){//没有报警 补上置零信息
        QJsonObject obj;
        obj.insert("WarningID", QString(""));
        obj.insert("WarningStr", 0);
        obj.insert("WarningLevel", -1);
        obj.insert("att1", 0);
        wData.insert(i , obj);
    }


    QList<QString> Json_WD;
    for(int i=0;i<wData.count();i++)
    {
        Json_WD.append(convertQJsontoQString(wData.at(i).toObject()));
    }
    //mainWindow->show_Json_WD(Json_WD);

    return wData;
}

/**
 * @brief convert QJsonObject to QString
 */
QString PushHmi::convertQJsontoQString(QJsonObject JsonObject)
{
    QJsonDocument doc;
    doc.setObject(JsonObject);
    QByteArray byte_array = doc.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    return json_str;
}
