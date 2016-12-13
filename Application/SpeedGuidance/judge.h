#ifndef JUDGE_H
#define JUDGE_H

#include "SpeedGuidance.h"

#include "WorldModel/world.h"

class Judge_guidance: public QObject
{
    Q_OBJECT
public:
    void Initialize();
private slots:
    void trigger_judge(void);
};

#endif // JUDGE_H
