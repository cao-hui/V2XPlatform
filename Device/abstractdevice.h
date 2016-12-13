#pragma once

#include <QByteArray>

class AbstractDevice
{
public:
    virtual void Initialize() = 0;
    virtual void WaitForDevice() = 0;
    virtual void Write(const QByteArray& data);
};

