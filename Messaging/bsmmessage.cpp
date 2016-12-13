#include "bsmmessage.h"
#include <QDebug>

QByteArray BSMMessage::Encode(BSMBlob& blob)
{
    int carID = blob.carID;
    double latitude = blob.latitude;
    double longitude = blob.longitude;
    double speed = blob.speed;
    double heading = blob.heading;
    double wheelAngle = blob.wheelAngle;
    double acceleration = blob.acceleration;
    bool carBrake = blob.wheelAngle;

    BasicSafetyMessage* bsmmessage = (BasicSafetyMessage*)CALLOC(1,sizeof(BasicSafetyMessage));
    bsmmessage->msgID = DSRCmsgID_basicSafetyMessage;
    int newcarID = carID;
    int newlatitude =  (int)round(latitude*1000000);
    int newlongitude =  (int)round(longitude*1000000);
    int newspeed = (int)round(speed/3.6*50);
    int newheading = (int)round(heading/0.0125);
    int newwheelAngle = (int)round(wheelAngle/1.5);
    int newacceleration = (int)round(acceleration*100);
    char buf[38] = {0};
    //id (2 - 5)
    buf[4] = newcarID & 0xFF;
    buf[3] = (newcarID & 0xFF00)>>8;
    buf[2] = (newcarID & 0xFF0000)>>16;
    buf[1] = (newcarID & 0xFF000000)>>24;

    //lat (8 - 11)
    if(newlatitude>900000000 || newlatitude < -900000000) //range: -900000000 ~ 900000000,unavailable:900000001
    {
        newlatitude = 900000001;
    }
    buf[10] = newlatitude & 0xFF;
    buf[9] = (newlatitude & 0xFF00)>>8;
    buf[8] = (newlatitude & 0xFF0000)>>16;
    buf[7] = (newlatitude & 0xFF000000)>>24;

    //long (12 - 15)
    if(newlongitude>1800000000 || newlongitude<-1800000000) //range: -1800000000 ~ 1800000000,unavailable:1800000001
    {
        newlongitude = 1800000001;
    }
    buf[14] = newlongitude & 0xFF;
    buf[13] = (newlongitude & 0xFF00)>>8;
    buf[12] = (newlongitude & 0xFF0000)>>16;
    buf[11] = (newlongitude & 0xFF000000)>>24;

    //speed (22 - 23)                //range: 0 ~ 8192
    buf[22] = newspeed & 0xFF;
    buf[21] = (newspeed & 0x1F00)>>8;

    //Heading (24 - 25)
    if(newheading<0 || newheading>28799) //range:0 ~ 28799,unavailable:28800
    {
        newheading = 28800;
    }
    buf[24] = newheading & 0xFF;
    buf[23] = (newheading & 0xFF00)>>8;

    //angle (26)
    if(newwheelAngle<-126 || newwheelAngle>126) //range: -126 ~ 126,unavailable:127
    {
        newwheelAngle = 127;
    }
    buf[25] = newwheelAngle & 0xFF;

    //acce(27 - 28)
    if(newacceleration<0 || newacceleration>5000)
    {
        newacceleration = -1;
    }
    buf[27] = newacceleration & 0xFF;
    buf[26] = (newacceleration & 0xFF00)>>8;

    OCTET_STRING_fromBuf(&bsmmessage->blob1, buf, 38);

    if(carBrake)
    {
        bsmmessage->safetyExt = (VehicleSafetyExtension*)CALLOC(1,sizeof(VehicleSafetyExtension));
        bsmmessage->safetyExt->events = (EventFlags_t*)CALLOC(1,sizeof(EventFlags_t));
        *(bsmmessage->safetyExt->events) = 128;
    }

    char message[1024];
    asn_enc_rval_t er;
    er = der_encode_to_buffer(&asn_DEF_BasicSafetyMessage, bsmmessage, message, 1024);
    asn_DEF_BasicSafetyMessage.free_struct(&asn_DEF_BasicSafetyMessage, bsmmessage, 0);
    if(er.encoded != -1)
    {
        return QByteArray(message, er.encoded);
    } else {
        return QByteArray();
    }
}

BSMBlob BSMMessage::Decode(QByteArray data)
{
    BasicSafetyMessage *bsmmsg = 0;
    asn_dec_rval_t rval;
    rval = asn_DEF_BasicSafetyMessage.ber_decoder(0,
                                                  &asn_DEF_BasicSafetyMessage,
                                                  (void **) &bsmmsg, /* Decoder moves the pointer */
                                                  data.data(), data.size(), 0);
    BSMBlob blob;
    if(rval.code == RC_OK)
    {
        int  tempcarID = 0;
        int  templatitude = 0;
        int  templongitude = 0;
        qint16 tempspeed = 0;
        qint16 tempheading = 0;
        qint16 tempacceleration = 0;
        qint8  tempwheelAngle = 0;

        //id (2 - 5)
        tempcarID = tempcarID | ((int)bsmmsg->blob1.buf[1])<<24;
        tempcarID = tempcarID | ((int)bsmmsg->blob1.buf[2])<<16;
        tempcarID = tempcarID | ((int)bsmmsg->blob1.buf[3])<<8;
        tempcarID = tempcarID | (int)bsmmsg->blob1.buf[4];

        //lat (8 - 11)
        templatitude = templatitude | ((int)bsmmsg->blob1.buf[7])<<24;
        templatitude = templatitude | ((int)bsmmsg->blob1.buf[8])<<16;
        templatitude = templatitude | ((int)bsmmsg->blob1.buf[9])<<8;
        templatitude = templatitude | (int)bsmmsg->blob1.buf[10];


        //long (12 - 15)
        templongitude = templongitude | ((int)bsmmsg->blob1.buf[11])<<24;
        templongitude = templongitude | ((int)bsmmsg->blob1.buf[12])<<16;
        templongitude = templongitude | ((int)bsmmsg->blob1.buf[13])<<8;
        templongitude = templongitude | (int)bsmmsg->blob1.buf[14];


        //speed (22 - 23)
        tempspeed = tempspeed | ((int)bsmmsg->blob1.buf[21] & 0x1F)<<8;
        tempspeed = tempspeed | (int)bsmmsg->blob1.buf[22];


        //Heading (24 - 25)
        tempheading = tempheading | ((int)bsmmsg->blob1.buf[23])<<8;
        tempheading = tempheading | (int)bsmmsg->blob1.buf[24];


        //angle (26)
        tempwheelAngle = tempwheelAngle | (int)bsmmsg->blob1.buf[25];

        //acceleration (27 - 28)
        tempacceleration = tempacceleration | ((int)bsmmsg->blob1.buf[26])<<8;
        tempacceleration = tempacceleration | ((int)bsmmsg->blob1.buf[27]);

        //safetyEvent:Brake
        if(bsmmsg->safetyExt !=NULL && bsmmsg->safetyExt->events!=NULL && *(bsmmsg->safetyExt->events) == 128)
        {
            blob.carBrake = true;
        }

        blob.carID = tempcarID;
        blob.latitude = templatitude/1000000.0;
        blob.longitude = templongitude/1000000.0;
        blob.speed = tempspeed/50.0*3.6;
        blob.heading = tempheading*0.0125;
        blob.wheelAngle = tempwheelAngle*1.5;
        blob.acceleration = tempacceleration/100.0;
    }

    BSMMessage::Free(bsmmsg);
    return blob;
}

void BSMMessage::Free(BasicSafetyMessage *message)
{
    if(message != NULL)
        asn_DEF_BasicSafetyMessage.free_struct(&asn_DEF_BasicSafetyMessage, message, 0);
}
