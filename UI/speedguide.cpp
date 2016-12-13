#include "speedguide.h"

speedGuide::speedGuide(QWidget *parent) : QWidget(parent)
{
    label_suggest = new label_modeOne(QString(" "));
    QLabel *lLabel_suggestSpeed = new QLabel(QString("Advanced Speed(km/h):"));
    QLabel *lLabel_TlightState = new QLabel(QString("Traffic Light:"));
    QLabel *lLabel_rTime = new QLabel(QString("Light Time(s):"));
    QLabel *lLabel_disToLine = new QLabel(QString("Distance(m):"));

    rLabel_suggestSpeed = new label_modeOne(QString(" "));
    rLabel_TlightState = new label_modeOne(QString(" "));
    rLabel_rTime = new label_modeOne(QString(" "));
    rLabel_disToLine = new label_modeOne(QString(" "));

    QVBoxLayout *leftVBox = new QVBoxLayout();
    QVBoxLayout *rightVBox = new QVBoxLayout();
    QVBoxLayout *upVBox = new QVBoxLayout();
    QHBoxLayout *downVBox = new QHBoxLayout();
    QVBoxLayout *mainVBox = new QVBoxLayout();

    leftVBox->addWidget(lLabel_suggestSpeed);
    leftVBox->addWidget(lLabel_TlightState);
    leftVBox->addWidget(lLabel_rTime);
    leftVBox->addWidget(lLabel_disToLine);
    rightVBox->addWidget(rLabel_suggestSpeed);
    rightVBox->addWidget(rLabel_TlightState);
    rightVBox->addWidget(rLabel_rTime);
    rightVBox->addWidget(rLabel_disToLine);
    upVBox->addWidget(label_suggest);
    downVBox->addLayout(leftVBox);
    downVBox->addLayout(rightVBox);
    mainVBox->addLayout(upVBox);
    mainVBox->addLayout(downVBox);
    mainVBox->setStretch(0,1);
    mainVBox->setStretch(1,3);

    this->setLayout(mainVBox);
}

void speedGuide::refresh(mark_sugestion m)
{
    rLabel_disToLine->setText(QString::number(m.Distance));
    //输出交通灯的状态和剩余时间
    if(m.Light_status==3)
    {
        rLabel_TlightState->setText("绿灯");
        rLabel_rTime->setText(QString::number(m.Time_left));
    }
    else if(m.Light_status==2)
    {
        rLabel_TlightState->setText("黄灯");
        rLabel_rTime->setText(QString::number(m.Time_left));
    }
    else if(m.Light_status==1)
    {
        rLabel_TlightState->setText("红灯");
        rLabel_rTime->setText(QString::number(m.Time_left));
    }

    switch (m.Mark) {
    case 0:
        label_suggest->setText("请减速至路口停车等红灯");//提示过不了路口，建议减速停车
        rLabel_suggestSpeed->clear();
        break;
    case 1:
        label_suggest->setText("请按当前车速行驶，也可参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));
        break;
    case 2:
        label_suggest->setText("请缓加速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));//提示缓加速，给出建议车速advise_speed
        break;
    case 3:
        label_suggest->setText("请加速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));//提示加速，给出建议车速advise_speed
        break;
    case 4:
        label_suggest->setText("请缓减速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed)); //提示缓减速，给出建议车速advise_speed
        break;
    case 5:
        label_suggest->setText("请减速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed)); //提示减速，给出建议车速advise_speed
        break;
    case 6:
        label_suggest->setText("请立刻！！！行驶至路口停车！！！");
        rLabel_suggestSpeed->clear();
        //qDebug()<<"666666";
        //ui->guidance_lineEdit->setText(QString::number(m1.Advise_speed)); //提示立刻减速到路口
        break;
    case 7:
        label_suggest->setText("请按当前车速行驶，也可参考建议车速");//提示按当前车速行驶，给出建议车速advise_speed
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));
        break;
    case 8:
        label_suggest->setText("请缓加速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));//提示缓加速，给出建议车速advise_speed
        break;
    case 9:
        label_suggest->setText("请加速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));//提示加速，给出建议车速advise_speed
        break;
    case 10:
        label_suggest->setText("请缓减速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed)); //提示缓减速，给出建议车速advise_speed
        break;
    case 11:
        label_suggest->setText("请减速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed)); //提示减速，给出建议车速advise_speed
        break;
    case 12:
        label_suggest->setText("请立刻！！！行驶至路口停车！！！");
        rLabel_suggestSpeed->clear();
        //qDebug()<<"121212";
        break;
    case 13:
        label_suggest->setText("请按当前车速行驶，也可参考建议车速");//提示按当前车速行驶，给出建议车速advise_speed
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));
        break;
    case 14:
        label_suggest->setText("请缓加速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));//提示缓加速，给出建议车速advise_speed
        break;
    case 15:
        label_suggest->setText("请加速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed));//提示加速，给出建议车速advise_speed
        break;
    case 16:
        label_suggest->setText("请缓减速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed)); //提示缓减速，给出建议车速advise_speed
        break;
    case 17:
        label_suggest->setText("请减速，需参考建议车速");
        rLabel_suggestSpeed->setText(QString::number(m.Advise_speed)); //提示减速，给出建议车速advise_speed
        break;
    default:
        break;
    }

}

void speedGuide::clear()
{
    label_suggest->setText("车辆已过路口/距离太远");
    rLabel_disToLine->clear();
    rLabel_rTime->clear();
    rLabel_suggestSpeed->clear();
    rLabel_TlightState->clear();
}
