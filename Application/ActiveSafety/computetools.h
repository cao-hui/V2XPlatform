#pragma once

#include "carData.h"
#include <QList>
#include <QPoint>
#include "qmath.h"


#define PI 3.141592653

double GPStoAxis(double angle); //GPS航向角变为直角坐标系航向角

void GPStoRecSys(carData &selfVehicle, carData &otherVehicle); //坐标转换

bool Intact(carData tempCarData);//判断车辆状态发生突变
void CenterPointPre(carData &tempCarData);//轨迹预测
void MovePoint_Present(carData &tempCarData);//实时轨迹的中心点求解

void MovePoint_erfen(carData &tempCarData);//二分法轨迹预测
void Calculate_s(double s[5][5],double x[],int m);  //计算拟合方程组的系数s
void Calculate_t(double t[],double x[],double y[],int m);   //计算拟合方程组的系数t
void DecoMatrix_LU(double L[5][5],double V[5][5],double s[5][5],int m);  //LU分解
void Calculate_a(double L[5][5],double V[5][5],double t[],double x[],int m);//计算拟合多项式的系数



void trajectoryPreRect(carData &tempCardata);//某一时刻的车辆矩形坐标
