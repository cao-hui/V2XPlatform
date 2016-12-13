#ifndef SPEEDGUIDE_H
#define SPEEDGUIDE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Application/SpeedGuidance/SpeedGuidance.h"
#include "sdatawidget.h"

class speedGuide : public QWidget
{
    Q_OBJECT
public:
    explicit speedGuide(QWidget *parent = 0);
    void refresh(mark_sugestion m);
    void clear();

private:
    label_modeOne *label_suggest;
    label_modeOne *rLabel_suggestSpeed;
    label_modeOne *rLabel_TlightState;
    label_modeOne *rLabel_rTime;
    label_modeOne *rLabel_disToLine;

signals:

public slots:
};

#endif // SPEEDGUIDE_H
