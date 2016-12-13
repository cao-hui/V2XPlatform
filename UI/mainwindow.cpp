#include "mainwindow.h"

mainwindow * GMainWindow = NULL;

mainwindow::mainwindow(QWidget *parent) :
    QWidget(parent)
{
    this->resize(800,500);

    sCardata = new sDataWidget();
    spdGuide = new speedGuide();
    pWidget = new paintWidget();
    dTable = new dataTable();

    tjPic = new QLabel();
    tjPic->setScaledContents(true);
    playCtrBtn = new QPushButton("Play");
    playCtrBtn->connect(playCtrBtn,SIGNAL(clicked()),this,SLOT(click_playCtr()));

    //布局管理
    QVBoxLayout *tjPicturePart = new QVBoxLayout();
    QVBoxLayout *funcButtonPart = new QVBoxLayout();
    QVBoxLayout *tjPicAndFuncBtnPart = new QVBoxLayout();
    QVBoxLayout *selfCarPart = new QVBoxLayout();
    QVBoxLayout *speedGuidePart = new QVBoxLayout();
    QVBoxLayout *pWidgetPart = new QVBoxLayout();
    QVBoxLayout *dTablePart = new QVBoxLayout();
    QHBoxLayout *upPart = new QHBoxLayout();
    QVBoxLayout *mainPart = new QVBoxLayout();

    //Part one
    //tjPic->setPixmap(QPixmap::fromImage(QImage("../../Source/tj.jpg")));
    tjPicturePart->addWidget(tjPic);
    funcButtonPart->addWidget(playCtrBtn);
    tjPicAndFuncBtnPart->addLayout(tjPicturePart);
    tjPicAndFuncBtnPart->addLayout(funcButtonPart);
    //Part two
    selfCarPart->addWidget(sCardata);
    speedGuidePart->addWidget(spdGuide);
    //Part three
    pWidgetPart->addWidget(pWidget);
    //Part four
    dTablePart->addWidget(dTable);

    //QGroupBox
    funcGroup = new QGroupBox("Function");
    selfCarGroup = new QGroupBox("Self Car");
    speedGuideGroup = new QGroupBox("Speed Guide");
    paintGroup = new QGroupBox("Car Path");
    wholeCarGroup = new QGroupBox("All Cars");
    funcGroup->setLayout(tjPicAndFuncBtnPart);
    selfCarGroup->setLayout(selfCarPart);
    speedGuideGroup->setLayout(speedGuidePart);
    paintGroup->setLayout(pWidgetPart);
    wholeCarGroup->setLayout(dTablePart);

    //整合布局
    upPart->addWidget(funcGroup);
    upPart->addWidget(selfCarGroup);
    upPart->addWidget(speedGuideGroup);
    upPart->addWidget(paintGroup);
    upPart->setStretch(0,1);
    upPart->setStretch(1,2);
    upPart->setStretch(2,2);
    upPart->setStretch(3,2);

    mainPart->addLayout(upPart);
    mainPart->addWidget(wholeCarGroup);
    mainPart->setStretch(0,1);
    mainPart->setStretch(1,1);

    this->setLayout(mainPart);
}

mainwindow::~mainwindow()
{

}


void mainwindow::click_playCtr()
{
    GPlayer->IsGlobalPaused = !GPlayer->IsGlobalPaused;
    qDebug()<<GPlayer->IsGlobalPaused;
}
