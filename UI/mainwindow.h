#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintwidget.h"
#include "datatable.h"
#include <QtGui>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "sdatawidget.h"
#include "speedguide.h"
#include "Recording/player.h"
#include <QPushButton>
#include <QGroupBox>

class mainwindow : public QWidget
{
    Q_OBJECT
public:
    explicit mainwindow(QWidget *parent = 0);
    ~mainwindow();

    void addObjectsToMap(int id, QVector<QPointF> path)
    {pWidget->addObjects(id,path);}
    void refresh_rawData(){dTable->refresh();sCardata->refresh();}
    void refresh_sGuide(mark_sugestion m){spdGuide->refresh(m);}
    void clear_sGuide(){spdGuide->clear();}

private:
    paintWidget *pWidget;
    dataTable *dTable;
    sDataWidget *sCardata;
    speedGuide *spdGuide;

    QLabel *tjPic;
    QPushButton *playCtrBtn;

    QGroupBox *funcGroup;
    QGroupBox *selfCarGroup;
    QGroupBox *speedGuideGroup;
    QGroupBox *paintGroup;
    QGroupBox *wholeCarGroup;

signals:

public slots:
    void click_playCtr();
};

extern mainwindow *GMainWindow;

#endif // MAINWINDOW_H
