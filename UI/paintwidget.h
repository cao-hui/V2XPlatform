#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMap>
#include <QVector>
#include <QPainterPath>

class paintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit paintWidget(QWidget *parent = 0);
    ~paintWidget();

    void setDrawScale(int ds){draw_Scale = ds;}
    void setLengthUnit(int lu){lengthUnit = lu;}
    void addObjects(int id, QVector<QPointF> path);

private:
    int draw_Scale;
    int lengthUnit;
    QMap<int, QVector<QPointF> > objects;
    QMap<int,int> survivalTime;
    QPixmap drawBG();
    QPointF coorTrans(QPointF pf);

protected:
    void resizeEvent(QResizeEvent * event) override;
    void paintEvent(QPaintEvent * event) override;
};

#endif // PAINTWIDGET_H
