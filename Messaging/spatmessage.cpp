#include "spatmessage.h"
#include <QDebug>

SPATLightState ColorIntToEnum(int color)
{
    //qDebug("color:%d",color);
    switch (color)
    {
    case 1:
        return green;
    case 2:
        return yellow;
    case 4:
        return red;
    default:
        return erro;
    }
}

SPATIntersectionState SPATMessage::Decode(QByteArray SPATData)
{
    SPAT_t *mySPAT = 0;
    asn_dec_rval_t rval;
    int id = 0;
    int lanes = 0;
    int ttime=0;
    QList<SPATMovementState> LightStates;
    SPATIntersectionState result;

    rval = asn_DEF_SPAT.ber_decoder(0, &asn_DEF_SPAT, (void **)&mySPAT, SPATData, SPATData.size (),0);

    if(rval.code != RC_OK)
        return result;

    if(mySPAT->intersections.list.array[0]->id.size == 2)
        id = htons((*(short *) mySPAT->intersections.list.array[0]->id.buf));
    else
        return result;

    lanes = *mySPAT->intersections.list.array[0]->lanesCnt;

    SPATMovementState lightStatTmp;//涓存椂鍙橀噺

    if(lanes % 3)
        return result;
    else
        lanes = lanes/3;

    for(int i=0;i<lanes;i++)
    {
        //qDebug("i:%d",i);
        //left
        //qDebug("left:%d",mySPAT->intersections.list.array[0]->states.list.array[i*3]->laneSet.size);
       // qDebug("left:%d",mySPAT->intersections.list.array[0]->states.list.array[i*3]->timeToChange);
        if(mySPAT->intersections.list.array[0]->states.list.array[i*3]->laneSet.size == 1)
            lightStatTmp.laneSet = mySPAT->intersections.list.array[0]->states.list.array[i*3]->laneSet.buf[0];
        else
            return result;

        if((lightStatTmp.leftState
                = ColorIntToEnum(*mySPAT->intersections.list.array[0]->states.list.array[i*3]->currState))
                == erro)
        return result;

        lightStatTmp.leftTimeToChange = mySPAT->intersections.list.array[0]->states.list.array[i*3]->timeToChange;
        ttime=*(mySPAT->intersections.list.array[0]->states.list.array[i*3]->pedCount);
                lightStatTmp.leftTime[0]=ttime/65536;
                ttime=ttime%65536;
                lightStatTmp.leftTime[1]=ttime/256;
                lightStatTmp.leftTime[2]=ttime=ttime%256;
        //qDebug("left");

        //straight
        if(mySPAT->intersections.list.array[0]->states.list.array[i*3+1]->laneSet.buf[0] != lightStatTmp.laneSet)
            return result;

        if((lightStatTmp.straightState
                = ColorIntToEnum(*mySPAT->intersections.list.array[0]->states.list.array[i*3+1]->currState))
                == erro)
            return result;
        lightStatTmp.straightTimeToChange =
                mySPAT->intersections.list.array[0]->states.list.array[i*3+1]->timeToChange;
        ttime=*(mySPAT->intersections.list.array[0]->states.list.array[i*3+1]->pedCount);
                lightStatTmp.straightTime[0]=ttime/65536;
                ttime=ttime%65536;
                lightStatTmp.straightTime[1]=ttime/256;
                lightStatTmp.straightTime[2]=ttime=ttime%256;
       //qDebug("str");

        //right
        if(mySPAT->intersections.list.array[0]->states.list.array[i*3+2]->laneSet.buf[0] != lightStatTmp.laneSet)
            return result;

        if((lightStatTmp.rightState
                = ColorIntToEnum(*mySPAT->intersections.list.array[0]->states.list.array[i*3+2]->currState))
                == erro)
            return result;

        lightStatTmp.rightTimeToChange = mySPAT->intersections.list.array[0]->states.list.array[i*3+2]->timeToChange;
        ttime=*(mySPAT->intersections.list.array[0]->states.list.array[i*3+2]->pedCount);
             lightStatTmp.rightTime[0]=ttime/65536;
             ttime=ttime%65536;
             lightStatTmp.rightTime[1]=ttime/256;
             lightStatTmp.rightTime[2]=ttime=ttime%256;
        //qDebug("right");

        LightStates.append(lightStatTmp);

    }

    asn_DEF_SPAT.free_struct(&asn_DEF_SPAT, mySPAT, 0);

    result.id = id;
    result.laneCount = lanes;
    result.states = LightStates;

    return result;
}
