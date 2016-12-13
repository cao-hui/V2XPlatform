#include "computetools.h"
#include <QDebug>

/*
 *  功能:坐标转换
 *  将GPS坐标转换为直角坐标
 * */
void GPStoRecSys(carData &selfVehicle, carData &otherVehicle)
{
    //航向角的转换
    selfVehicle.heading_Axis = GPStoAxis(selfVehicle.heading_GPS);
    otherVehicle.heading_Axis = GPStoAxis(otherVehicle.heading_GPS);

    //地球半径约为 6371.004km
    //经纬度系数
    double LatiRatio = 111712.69150641056;   //6371004*PI/180
    double LonRatio = LatiRatio*cos(selfVehicle.VLatitude/180.0*PI);

    double x;
    double y;
    int i;
    //自车起点坐标
    selfVehicle.startPoint = QPointF(0,0);
    for(i=0;i<5;i++)
    {
        x = (selfVehicle.carHistoryLong[i] - selfVehicle.VLongitude) * LonRatio;
        y = (selfVehicle.carHistoryLati[i] - selfVehicle.VLatitude) * LatiRatio;
        selfVehicle.carHistoryPoint[i] = QPointF(x,y);
    }

    //对其他车辆坐标进行转换
    x = (otherVehicle.VLongitude  - selfVehicle.VLongitude) * LonRatio;
    y = (otherVehicle.VLatitude - selfVehicle.VLatitude) * LatiRatio;
    otherVehicle.startPoint = QPointF(x,y);


    //对其他车辆坐标进行转换
//    for(i =0; i<6;i++)
//    {
//        x = (otherVehicle.carHistoryLong[i] - selfVehicle.VLongitude) * LonRatio;
//        y = (otherVehicle.carHistoryLati[i] - selfVehicle.VLatitude) * LatiRatio;
//        otherVehicle.carHistoryPoint[i] = QPointF(x,y);
//    }
//    otherVehicle.startPoint = otherVehicle.carHistoryPoint[5];

}


/* 坐标系转换
   GPS航向角->直角坐标系航向角 */
double GPStoAxis(double angle)
{
    //原来的GPS为正北为0，现在改为直角坐标的正东为0
    angle = 90 - angle;
    if(angle < -180)
    {
        angle = angle + 360;
    }
}

// 状态是否发生突变
bool Intact(carData tempCarData)
{
    bool Status = true;
    for(int i= 0;i<5;i++)
    {
        if(tempCarData.wheelAngle - tempCarData.carHistoryHeading[i]> 0.1 )
            Status = false;
    }
    if(tempCarData.BrakeStatus ==true)
        Status =false;
    return Status;
}

void CenterPointPre(carData &tempCarData)
{
    if(Intact(tempCarData))
        MovePoint_erfen(tempCarData);
    else
        MovePoint_Present(tempCarData);
}

//根据当前点的状态进行预测
void MovePoint_Present(carData &tempCarData)
{
    double arrowTime = 0.02;//间隔时间
    int arrowNum = 300; //检测点的个数
    double r; //转弯的半径
    double Angle;//每段时间的角度变化值
    double Angle0;//初始角度
    double x1;//下一个点的坐标
    double y1;
    double tempAngle;
    double l;//弦长
    QPointF temp0;

    temp0 = tempCarData.startPoint;
    Angle0 = tempCarData.heading_Axis*PI/180;
    r =qFabs(tempCarData.vehicleWheelDis/(qSin(tempCarData.wheelAngle*PI/180)+0.000001));//转弯半径

    Angle = tempCarData.Speedmps*arrowTime/r; //每个间隔段的角度,单位是 弧度
//    l = 2*r*qSin(Angle/2);//每个间隔时间的弦长

    //根据不同的轮转角进行选取轨迹的圆心
    if(tempCarData.wheelAngle >= 0)
    {

        for(int i = 1;i<=arrowNum;i++)
        {
            tempAngle = Angle0 - i*Angle/2; //单位为 hude;
            l = 2*r*qSin(i*Angle/2);
            x1 = tempCarData.startPoint.x()+l*qCos(tempAngle);
            y1 = tempCarData.startPoint.y()+l*qCos(tempAngle);
            temp0.setX(x1);
            temp0.setY(y1);
            tempCarData.carLine.append(temp0);

        }
    }
    else if(tempCarData.wheelAngle < 0)
    {
        for(int i = 1;i<=arrowNum;i++)
        {
            tempAngle = Angle0 + i*Angle/2;//单位为 hude;
            l = 2*r*qSin(i*Angle/2);
            x1 = tempCarData.startPoint.x() + l*qCos(tempAngle);
            y1 = tempCarData.startPoint.y() + l*qSin(tempAngle);
            temp0.setX(x1);
            temp0.setY(y1);
            tempCarData.carLine.append(temp0);
        }
    }
}


//二分法轨迹预测
void MovePoint_erfen(carData &tempCarData)
{

    double  k[6],x[6],y[6];//初始已知点的坐标,6个
    double pre_x[300],pre_y[300];
    double a[5];//拟合多项式的系数 4+1
    double s[5][5]={0},t[5]={0},L[5][5],V[5][5];//4+1
    int i;
    int j;
    QPointF temp;

    //预测X的坐标值
    for(i=0;i<6;i++)
    {
        k[i] = i;
        x[i] = tempCarData.carHistoryPoint[i].x();
        y[i] = tempCarData.carHistoryPoint[i].y();
    }
    Calculate_s(s,k,4);//计算方程组的系数S，4为拟合阶数
    Calculate_t(t,k,x,4);//计算拟合 X 方程组的系数t，4为拟合阶数
    DecoMatrix_LU(L,V,s,4);//LV分解
    Calculate_a(L,V,t,a,4);//计算拟合多项式的系数，4为拟合阶数
    j = 6;
    for(i = 0;i<300;i++)
    {
        pre_x[i] = a[0]+a[1]*j+a[2]*j*j+a[3]*j*j*j;
        j++;
    }

    //预测Y的坐标值
    Calculate_t(t,k,y,4);//计算拟合 Y 方程组的系数t，4为拟合阶数
    Calculate_a(L,V,t,a,4);//计算拟合多项式的系数，4为拟合阶数
    j=6;
    for(i = 0;i<300;i++)
    {
        pre_y[i] = a[0]+a[1]*j+a[2]*j*j+a[3]*j*j*j;
        j++;
    }

    for(i=0;i<300;i++)
    {
        temp.setX(pre_x[i]);
        temp.setY(pre_y[i]);
        tempCarData.carLine.append(temp);
    }
}

//二分法轨迹预测_计算方程组的系数S
void Calculate_s(double s[5][5],double x[],int m)
{
    int i,j;
    for (i=0;i<=m;i++)
    {
        for(j=0;j<=m;j++)
            s[i][j]=0;

    }
    for ( i=0;i<=m;i++)
        for (j=0;j<=m;j++)
        {
            for(int k=0;k<6;k++)
            {
                s[i][j]+=pow(x[k],i+j);
            }
        }
}

//二分法轨迹预测_拟合方程组的系数t
void Calculate_t(double t[],double x[],double y[],int m)
{
    int i;
    for (i=0;i<=m;i++)
    {
        t[i]=0;
    }

    for (i=0;i<=m;i++)
        for (int j=0;j<6;j++)
        {
            t[i]+=y[j]*pow(x[j],i);
        }
}

//二分法轨迹预测_LU分解,5为拟合阶数+1
void DecoMatrix_LU(double L[5][5],double V[5][5],double s[5][5],int m)
{
    double sum;
    int i;
    int j;
    int k;
    for (i=0;i<=m;i++)
        for(int j=0;j<=m;j++ )
    {
            L[i][j]=0;
            V[i][j]=0;
    }
    for(i=0;i<=m;i++)
    {
          L[i][i]=1;
    }
    for (i=0;i<=m;i++)
    {
        //计算U矩阵的第i行
        for (j=0;j<=m;j++)
        {
            sum=0;
            for (int k=0;k<=i-1;k++)
            {
                sum+=L[i][k]*V[k][j];
            }
            V[i][j]=s[i][j]-sum;
        }
        if(i<m)
        {
            //计算L矩阵的第i列
            for (j=1;j<=m;j++)
            {
                sum = 0;
                for (k=0;k<=i-1;k++)
                {
                 sum+=L[j][k]*V[k][i];
                }
               L[j][i]=(s[j][i]-sum)/V[i][i];
            }
        }
    }
}

//二分法轨迹预测_拟合多项式的系数，L[5][5]其中的5为拟合阶数+1
void Calculate_a(double L[5][5],double V[5][5],double t[],double x[],int m)
{
    double  y[5],sum;
    int i;
    int j;
    for (i=0;i<=m;i++)
    {
        sum=0;
        for (j=0;j<i;j++)
        {
           sum+=L[i][j]*y[j];
        }
        y[i]=t[i]-sum;
    }
    for (i=m;i>=0;i--)
    {
        sum=0;
        for (j=m;j>i;j--)
        {
            sum+=V[i][j]*x[j];
        }
        x[i]=(y[i]-sum)/V[i][i];
    }
}

/*
 * 某一时刻车辆矩形坐标
*/

void trajectoryPreRect(carData &tempCardata)
{
    double l;
    double d;
    double Angel1;
    double Angel2;
    QPointF center;
    l = tempCardata.vehicleLength/2.0;
    d = tempCardata.vehicleWidth/2.0;

    Angel1 = tempCardata.heading_Axis;
    Angel2 = tempCardata.heading_Axis+90;

    for(int i=0;i<300;i++)
    {
        center = tempCardata.carLine.at(i);

        tempCardata.rotateRect[i][0].setX(center.x()+l*qCos(Angel1*PI/180)-d*qCos(Angel2*PI/180));//UP_Left
        tempCardata.rotateRect[i][0].setY(center.y()+l*qSin(Angel1*PI/180)-d*qSin(Angel2*PI/180));

        tempCardata.rotateRect[i][1].setX(center.x()+l*qCos(Angel1*PI/180)+d*qCos(Angel2*PI/180));//UP_Right
        tempCardata.rotateRect[i][1].setY(center.y()+l*qSin(Angel1*PI/180)+d*qSin(Angel2*PI/180));

        tempCardata.rotateRect[i][2].setX(center.x()-l*qCos(Angel1*PI/180)+d*qCos(Angel2*PI/180));//Down_Right
        tempCardata.rotateRect[i][2].setY(center.y()-l*qSin(Angel1*PI/180)+d*qSin(Angel2*PI/180));

        tempCardata.rotateRect[i][3].setX(center.x()-l*qCos(Angel1*PI/180)-d*qCos(Angel2*PI/180));//Down_Left
        tempCardata.rotateRect[i][3].setY(center.y()-l*qSin(Angel1*PI/180)-d*qSin(Angel2*PI/180));
    }
}

