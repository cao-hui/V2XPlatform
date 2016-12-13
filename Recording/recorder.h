#pragma once

#include "v2xplatform.h"
#include <QObject>
#include <QThread>
#include <QUdpSocket>
#include <QDateTime>

class RecordingThread: public QThread
{
    Q_OBJECT
private:
    QUdpSocket* socket;
public:
    void run();
};

class Recorder: public QObject
{
    Q_OBJECT
private:
    RecordingThread recordingThread;
public:
    void StartRecording(QString recordDescription);
    void StopRecording();
    Recorder();
private slots:
    void onGPSDataRefreshed(PlatformTimestamp timestamp);
    void onCANDataRefreshed(PlatformTimestamp timestamp);
    void onDSRCDataRefreshed(PlatformTimestamp timestamp, QByteArray data);
};

extern Recorder* GRecorder;
