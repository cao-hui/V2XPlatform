#include "paintwidget.h"
#include <QDebug>
#include <QRect>
#include <QPointF>

paintWidget::paintWidget(QWidget *parent) :
    QWidget(parent),draw_Scale(10),lengthUnit(5)
{
    this->resize(400, 300);
    setAutoFillBackground(true);
}

paintWidget::~paintWidget()
{

}

void paintWidget::resizeEvent(QResizeEvent * event)
{
    //qDebug()<<"resizeEvent";
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(drawBG()));
    this->setPalette(palette);
}

void paintWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    if(objects.count() != 0)
    {
        for(auto it = objects.begin();it != objects.end();++ it)
        {
            if(it.value().count() > 1)
            {
                for(auto i = it.value().begin();i != (it.value().end() - 1); ++ i)
                {
                    painter.drawLine(coorTrans(*i),coorTrans(*(i+1)));
                }
            }
        }
    }
}

void paintWidget::addObjects(int id, QVector<QPointF> path)
{
    for(auto it = survivalTime.begin();it != survivalTime.end();)
    {
        --(it.value());
        if(it.value() < 0)
        {
            survivalTime.erase(it);
            objects.erase(objects.find(it.key()));
        }
        else
            ++it;
    }
    objects[id] = path;
    survivalTime[id] = 50;
    this->update();
}

QPixmap paintWidget::drawBG()
{
    //qDebug()<<"drawBG";
    QPixmap bg(this->size());
    bg.fill(Qt::white);
    QPainter painter(&bg);
    painter.setPen(Qt::black);
    painter.drawLine(QPointF(this->width()/2,0),QPointF(this->width()/2,this->height()));
    painter.drawLine(QPointF(0,this->height()/2),QPointF(this->width(),this->height()/2));
    for(int i=0,t=0;i<this->width()/2;i=i+lengthUnit*draw_Scale,t=t+lengthUnit)
    {
        painter.drawText(QPointF(this->width()/2 + i,this->height()/2),QString::number(t));
        painter.drawText(QPointF(this->width()/2 - i,this->height()/2),QString::number(-t));
    }
    for(int i=0,t=0;i<this->height()/2;i=i+lengthUnit*draw_Scale,t=t+lengthUnit)
    {
        painter.drawText(QPointF(this->width()/2,this->height()/2 + i),QString::number(-t));
        painter.drawText(QPointF(this->width()/2,this->height()/2 - i),QString::number(t));
    }
    return bg;
}

QPointF paintWidget::coorTrans(QPointF pf)
{
    pf.setX(pf.x()*draw_Scale + this->width()/2);
    pf.setY(this->height()/2 - pf.y()*draw_Scale);
    return pf;
}
