#ifndef SELFCARDATA_H
#define SELFCARDATA_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPalette>
#include "WorldModel/world.h"

class label_modeOne;

class sDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit sDataWidget(QWidget *parent = 0);
    ~sDataWidget();
    void refresh();

private:
    label_modeOne * carIdLabel;  //速度
    label_modeOne * longitudeLabel;
    label_modeOne * latitudeLabel;
    label_modeOne * headingLabel;  //航向角
    label_modeOne * speedLabel;
    label_modeOne * wheelAngleLabel;

signals:

public slots:
};

//white background, center alignment in both vertical and horizontal
//used to show dynamic data
class label_modeOne : public QLabel
{
    Q_OBJECT
public:
    label_modeOne()
        :QLabel()
    {
        QPalette palette;
        palette.setColor(QPalette::Background, QColor(255,255,255,255));
        this->setAutoFillBackground(true);
        this->setPalette(palette);
        this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    label_modeOne(const QString & text)
        :QLabel(text)
    {
        QPalette palette;
        palette.setColor(QPalette::Background, QColor(255,255,255,255));
        this->setAutoFillBackground(true);
        this->setPalette(palette);
        this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
private:
    //void resizeEvent(QResizeEvent * event) override;
    //void paintEvent(QPaintEvent * event) override;
};

#endif // SELFCARDATA_H
