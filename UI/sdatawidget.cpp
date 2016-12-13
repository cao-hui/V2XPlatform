#include "sdatawidget.h"
#include <QDebug>

sDataWidget::sDataWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *carIdTitle = new QLabel(QString("Car ID:"));
    QLabel *longitudeTitle = new QLabel(QString("Longitude:"));
    QLabel *latitudeTitle = new QLabel(QString("Latitude:"));
    QLabel *headingTitle = new QLabel(QString("Heading:"));
    QLabel *speedTitle = new QLabel(QString("Speed:"));
    QLabel *wheelAngleTitle = new QLabel(QString("Wheel Angle:"));

    carIdLabel = new label_modeOne(QString(" "));
    longitudeLabel = new label_modeOne(QString(" "));
    latitudeLabel = new label_modeOne(QString(" "));
    headingLabel = new label_modeOne(QString(" "));
    speedLabel = new label_modeOne(QString(" "));
    wheelAngleLabel = new label_modeOne(QString(" "));

    QVBoxLayout *lVBox = new QVBoxLayout();
    QVBoxLayout *rVBox = new QVBoxLayout();
    QHBoxLayout *mainHBox = new QHBoxLayout();

    lVBox->addWidget(carIdTitle);
    lVBox->addWidget(longitudeTitle);
    lVBox->addWidget(latitudeTitle);
    lVBox->addWidget(headingTitle);
    lVBox->addWidget(speedTitle);
    lVBox->addWidget(wheelAngleTitle);

    rVBox->addWidget(carIdLabel);
    rVBox->addWidget(longitudeLabel);
    rVBox->addWidget(latitudeLabel);
    rVBox->addWidget(headingLabel);
    rVBox->addWidget(speedLabel);
    rVBox->addWidget(wheelAngleLabel);

    mainHBox->addLayout(lVBox);
    mainHBox->addLayout(rVBox);
    mainHBox->setStretch(0,1);
    mainHBox->setStretch(1,1);

    this->setLayout(mainHBox);
}

sDataWidget::~sDataWidget()
{

}

void sDataWidget::refresh()
{
    //qDebug()<<QString::number(GWorld->Self->ID);
    carIdLabel->setText(QString::number(GWorld->Self->ID));
    longitudeLabel->setText(QString::number(GWorld->Self->Longitude,10,6));
    latitudeLabel->setText(QString::number(GWorld->Self->Latitude,10,6));
    headingLabel->setText(QString::number(GWorld->Self->Heading,10,6));
    speedLabel->setText(QString::number(GWorld->Self->Speed,10,6));
    wheelAngleLabel->setText(QString::number(GWorld->Self->WheelAngle,10,6));
}


//label_modeOne
//void label_modeOne::resizeEvent(QResizeEvent * event)
//{
//    int label_height = this->height();
//    //this->setFont(QFont("Arial",--fontsize));
//}

//void label_modeOne::paintEvent(QPaintEvent *event)
//{

//}
