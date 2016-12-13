#pragma once

#include <qthread.h>
#include <time.h>
#include <QMetaType>

class PlatformTimestamp
{
public:
    qint64 Seconds;
    qint64 Nanoseconds;

    PlatformTimestamp(timespec& ts)
        :Seconds(ts.tv_sec), Nanoseconds(ts.tv_nsec)
    {}

    PlatformTimestamp(int tv_sec, int tv_nsec)
        :Seconds(tv_sec), Nanoseconds(tv_nsec)
    {}

    PlatformTimestamp(qint64 tv_sec, qint64 tv_nsec)
        :Seconds(tv_sec), Nanoseconds(tv_nsec)
    {}

    PlatformTimestamp()
        :Seconds(0), Nanoseconds(0)
    {}

    PlatformTimestamp operator+(const PlatformTimestamp& operand);

    bool operator > (const PlatformTimestamp& operand) const;

    bool operator < (const PlatformTimestamp& operand) const;

    qint64 GetTotalNanoseconds();

    qint64 GetTotalMilliseconds();

    static PlatformTimestamp Diff(PlatformTimestamp start, PlatformTimestamp end);
    static PlatformTimestamp GetSystemMonotonicTime();
};

Q_DECLARE_METATYPE(PlatformTimestamp);


class V2XPlatform
{
private:
    PlatformTimestamp platformStartTime;
public:
    void Initialize(int argc, char *argv[]);
    PlatformTimestamp GetPlatformTime();
    PlatformTimestamp GetPlatformStartTime();
    V2XPlatform();
};

extern V2XPlatform* GV2XPlatform;

extern class MainWindow* mainWindow;
