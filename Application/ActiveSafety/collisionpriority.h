#ifndef COLLISIONPRIORITY_H
#define COLLISIONPRIORITY_H
#include <QList>

class CollisionPriority
{
public:
    CollisionPriority();
    QList<QString>  CollisionID;   //碰撞车辆的ID
    QList<QString>  CollisionType; //碰撞的类型
    QList<int>  CollisionLevel;    //碰撞级别
};

#endif // COLLISIONPRIORITY_H
